#include <Arduino.h>

#include "FS.h"
#include "LITTLEFS.h"

#include <EncButton.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TMCStepper.h>
#include <TimberWidget.h>

#include "global.h"
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
constexpr uint32_t kStepperTimerFrequencyHz = 50000;
constexpr uint64_t kStepperTimerAlarmTicks = 1;
constexpr uint32_t kLoopDelayMs = 1;
constexpr uint32_t kUiHeartbeatPeriodMs = 50;
constexpr uint32_t kDbTaskDelayMs = 500;
constexpr uint8_t kStatusPanelIndex = 3;
constexpr int kUiHeartbeatMax = 99;
constexpr float kMotorStepAngleDegrees = 1.8f;
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
    vibroAngle.init(10.0f);
    vibroFr.init(10.0f);
}

void initStepperTimer()
{
    stepperTimer = timerBegin(kStepperTimerFrequencyHz);
    timerAttachInterrupt(stepperTimer, &onTimer);
    timerAlarm(stepperTimer, kStepperTimerAlarmTicks, true, 0);
    timerStart(stepperTimer);
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
    portENTER_CRITICAL_ISR(&stepperMux);
    stepper.tick();
    portEXIT_CRITICAL_ISR(&stepperMux);
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
    delay(kLoopDelayMs);
    updateUiHeartbeat();
}
