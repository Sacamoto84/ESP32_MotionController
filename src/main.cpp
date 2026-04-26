#include <Arduino.h>

#include "FS.h"
#include "LITTLEFS.h"

#include <EncButton.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TMCStepper.h>
#include <TimberWidget.h>

#include "global.h"
#include "ina226Monitor.h"
#include "motionConfig.h"
#include "storage.h"

extern EncButton eb;

extern void lcdInit();
extern void encoderInit();

void onTimer();

using namespace TimberWidget;

TimberWidgets ui(Serial);

namespace
{
constexpr uint32_t kSerialBaudRate = 1000000;
constexpr uint32_t kSerialTxBufferSize = 4096;
constexpr uint32_t kLoopDelayMs = 1;
constexpr uint32_t kUiHeartbeatPeriodMs = 50;
constexpr uint32_t kDbTaskDelayMs = 500;
constexpr uint8_t kStatusPanelIndex = 3;
constexpr int kUiHeartbeatMax = 99;
constexpr char kUiAccentColor[] = "#36C36B";

hw_timer_t *stepperTimer = nullptr;
TaskHandle_t taskDb = nullptr;
int uiHeartbeatProgress = 0;
uint32_t uiHeartbeatTimer = 0;

void initUiDashboard()
{
    ui.to(0).badgeStyle("0", BadgeStyle::Ok);
    ui.to(1).badgeStyle("1", BadgeStyle::Ok);
    ui.to(2).badgeStyle("2", BadgeStyle::Ok);

    ui.to(kStatusPanelIndex).clearTerminal();
    ui.to(kStatusPanelIndex).badgeStyle("Ok", BadgeStyle::Ok);
    ui.to(kStatusPanelIndex).badgeStyle("Error", BadgeStyle::Error);
    ui.to(kStatusPanelIndex).badgeStyle("Info", BadgeStyle::Info);
    ui.to(kStatusPanelIndex).at(0).progress(0, "Battery", 100, kUiAccentColor, "72%");
}

void logStartupBanner()
{
    timber.i("--------------------------------");
    timber.i("Motion controller V1.0");
    timber.i("--------------------------------");
}

void initializeStates()
{
    tmcStepperEnable.init(0);
    tmcDriverChop.init(0);
    tmcDriverCurrent.init(1000);
    tmcDriverMicrostep.init(16);
    tmcInterpolation.init(0);
    tmcStepperMaxSpeed.init(2000);
    tmcStepperTarget.init(10);
    constDirection.init(1);
    constRps.init(1.0f);
    tmcFastStandstill.init(0);
    tmcTPwmThrs.init(0);
    tmcTPowerDown.init(10);
    tmcIrun.init(31);
    tmcIhold.init(16);
    tmcIholdDelay.init(6);
    tmcGlobalScaler.init(0);
    tmcFreewheel.init(0);
    tmcToff.init(8);
    tmcTbl.init(1);
    tmcHstrt.init(5);
    tmcHend.init(2);
    tmcChm.init(0);
    tmcDedge.init(0);
    tmcVhighfs.init(0);
    tmcVhighchm.init(0);
    tmcPwmAutoscale.init(1);
    tmcPwmAutograd.init(1);
    tmcPwmOfs.init(30);
    tmcPwmGrad.init(0);
    tmcPwmFreq.init(0);
    tmcPwmReg.init(4);
    tmcPwmLim.init(12);
    tmcSgt.init(0);
    tmcSfilt.init(0);
    tmcTCoolThrs.init(0);
    tmcTHigh.init(0);
    tmcSemin.init(0);
    tmcSemax.init(0);
    tmcSeup.init(0);
    tmcSedn.init(0);
    tmcSeimin.init(0);
    tmcVdcMin.init(0);
    tmcDcTime.init(0);
    tmcDcSg.init(0);
    tmcRecalibrate.init(0);
    tmcMultistepFilt.init(0);
    tmcStopEnable.init(0);
    tmcDiag0Error.init(0);
    tmcDiag0Otw.init(0);
    tmcDiag0Stall.init(0);
    tmcDiag1Stall.init(0);
    tmcDiag1Index.init(0);
    tmcDiag1Onstate.init(0);
    tmcDiag1StepsSkipped.init(0);
    tmcDiag0PushPull.init(0);
    tmcDiag1PushPull.init(0);
    tmcS2vsLevel.init(6);
    tmcS2gLevel.init(6);
    tmcShortFilter.init(1);
    tmcShortDelay.init(0);
    tmcBbmTime.init(0);
    tmcBbmClks.init(4);
    tmcOtSelect.init(0);
    tmcDrvStrength.init(2);
    tmcFiltIsense.init(0);
    vibroAngle.init(10.0f);
    vibroFr.init(10.0f);
}

void initStepperTimer()
{
    stepperTimer = timerBegin(kStepperTimerFrequencyHz);
    timerAttachInterrupt(stepperTimer, &onTimer);
    timerWrite(stepperTimer, 0);
    timerAlarm(stepperTimer, kStepperIdlePeriodUs, false, 0);
    timerStart(stepperTimer);
}

uint32_t IRAM_ATTR clampStepperTimerPeriod(uint32_t periodUs)
{
    if (periodUs < kStepperMinPeriodUs)
    {
        return kStepperMinPeriodUs;
    }

    return periodUs;
}

void IRAM_ATTR scheduleStepperTimer(uint32_t periodUs)
{
    if (stepperTimer == nullptr)
    {
        return;
    }

    timerWrite(stepperTimer, 0);
    timerAlarm(stepperTimer, clampStepperTimerPeriod(periodUs), false, 0);
}

void logDriverDiagnostics()
{
    const uint32_t driverStatus = driver.DRV_STATUS();
    Serial.print("DRV_STATUS: 0x");
    Serial.println(driverStatus, HEX);

    Serial.print("DRV_STATUS=0b");
    Serial.println(driver.DRV_STATUS(), BIN);

    Serial.print("version: ");
    Serial.println(driver.version());

    Serial.print("run status: ");
    Serial.println(driver.irun());

    Serial.print("run en stat: ");
    Serial.println(driver.encb_dcen_cfg4());

    Serial.print("toff: ");
    Serial.println(driver.toff());
}

int32_t calculateVibroTravelSteps()
{
    const float microstep = static_cast<float>(tmcDriverMicrostep.get());
    if (microstep <= 0.0f)
    {
        return 0;
    }

    const float stepAngle = kMotorStepAngleDegrees / microstep;
    const float travelSteps = vibroAngle.get() / stepAngle;
    return static_cast<int32_t>(travelSteps + 0.5f);
}

void handleVibroMode()
{
    const int32_t travelSteps = calculateVibroTravelSteps();
    int32_t endpointSteps = travelSteps / 2;
    if (travelSteps > 0 && endpointSteps < 1)
    {
        endpointSteps = 1;
    }

    vibroTarget.store(endpointSteps);

    portENTER_CRITICAL(&stepperMux);
    const bool ready = stepper.ready() || stepper.getStatus() == 0;
    portEXIT_CRITICAL(&stepperMux);

    if (!ready)
    {
        return;
    }

    const bool nextDirection = !vibroDir.load();
    vibroDir.store(nextDirection);
    const int32_t signedTarget = nextDirection ? endpointSteps : -endpointSteps;

    portENTER_CRITICAL(&stepperMux);
    stepper.setTarget(signedTarget);
    portEXIT_CRITICAL(&stepperMux);
}

void runCurrentMode()
{
    if (currentMode.get() == WorkMode::VIBRO)
    {
        handleVibroMode();
    }
}

void updateUiHeartbeat()
{
    const uint32_t now = millis();
    if (now - uiHeartbeatTimer < kUiHeartbeatPeriodMs)
    {
        return;
    }
    uiHeartbeatTimer = now;

    uiHeartbeatProgress += 1;
    if (uiHeartbeatProgress > kUiHeartbeatMax)
    {
        uiHeartbeatProgress = 0;
    }

    ui.to(kStatusPanelIndex).progress(uiHeartbeatProgress, "Battery", 100, kUiAccentColor, "72%");
}

[[noreturn]] void taskDbLoop(void *parameter)
{
    (void)parameter;

    for (;;)
    {
        tickStorage();
        delay(kDbTaskDelayMs);
    }
}

void initTaskDb()
{
    xTaskCreatePinnedToCore(taskDbLoop, "TaskDbLoop", 1000, nullptr, 5, &taskDb, 0);
}
} // namespace

void IRAM_ATTR onTimer()
{
    uint32_t nextPeriodUs = kStepperIdlePeriodUs;

    portENTER_CRITICAL_ISR(&stepperMux);
    if (stepper.getStatus() != 0)
    {
        stepper.tickManual();
        if (stepper.getStatus() != 0)
        {
            nextPeriodUs = stepper.getPeriod();
        }
    }
    portEXIT_CRITICAL_ISR(&stepperMux);

    scheduleStepperTimer(nextPeriodUs);
}

void setup()
{
    Serial.setTxBufferSize(kSerialTxBufferSize);
    Serial.begin(kSerialBaudRate);

    lcdInit();
    Timber::clear();
    initUiDashboard();
    logStartupBanner();

    encoderInit();
    tmcInit();
    initIna226Monitor();

    if (!LittleFS.begin(true))
    {
        timber.e("LittleFS mount failed");
    }
    else
    {
        initStorage();
    }

    observerAll();
    initializeStates();
    loadContinuousScreenSettings();
    loadTmc2160Settings();
    loadVibroScreenSettings();
    currentMode.notifyObservers();

    timber.i("Init values - MaxSpeed: %d, Target: %d, VibroFr: %.2f, VibroAngle: %.2f",
             tmcStepperMaxSpeed.get(), tmcStepperTarget.get(),
             vibroFr.get(), vibroAngle.get());

    initStepperTimer();
    logDriverDiagnostics();
    initTaskDb();
}

void loop()
{
    runCurrentMode();
    tickIna226Monitor();
    delay(kLoopDelayMs);
    updateUiHeartbeat();
}
