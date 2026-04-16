#include <Arduino.h>

#include <WiFi.h>
#include <ArduinoOTA.h>

#include "FS.h"
#include "LITTLEFS.h"

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <TMCStepper.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEClient.h>

#include <esp_task_wdt.h>

#include "global.h"

#include <EncButton.h>
#include <ble/ble.h>

#include <ESPAsyncWebServer.h>

constexpr bool erase{false};        // true writes 0xFF to all addresses,
                                    //   false performs write/read test
constexpr uint32_t totalKBytes{64}; // for read and write test functions
constexpr uint8_t btnStart{6};      // pin for start button

extern EncButton eb;

extern void lcdInit();

extern void encoderInit();

// Ble ble;

extern void initTaskDb();

bool shaft = false;

// esp8266/esp32
//  IRAM_ATTR void isr() {
//      eb.tickISR();
//  }

#define FREQUENCY 100000 // Частота меандра в Гц (1 кГц)

hw_timer_t *timer = nullptr;

// volatile bool state = false;

uint8_t dir = 1;


const char* WIFI_SSID = "TP-Link_BC0C";
const char* WIFI_PASS = "58133514";


void IRAM_ATTR onTimer()
{
    // state = !state; // Переключаем состояние

    //static uint32_t tickCount = 0;
    //tickCount++;
    //if (tickCount % 1000 == 0)
    //{
        //timber.i("DEBUG: onTimer tick count = %d", tickCount);
    //}
    stepper.tick();
}

void setup()
{
    /////////// esp_task_wdt_init(30, false);
    Serial.setTxBufferSize(4096);
    Serial.begin (1000000);
    
    lcdInit();

    Timber::clear();
    
    Serial.println("11111111111111111111");

    timber.i("--------------------------------");
    timber.i("Контроллер шагового мотора V1.0");
    timber.w("Ворнинг");
    timber.e("Ошибка");
    timber.colorStringln(15, 36, "Проверка 15 36");
    timber.i("--------------------------------");

    encoderInit();
    tmcInit();

    if (!LittleFS.begin(true)) timber.e("Ошибка при монтировании LittleFS");

    timber.i("------------ init ----------------");

    observerAll();

    tmcStepperEnable.init(0);
    tmcDriverChop.init(0);
    tmcDriverCurrent.init(1000);
    tmcDriverMicrostep.init(16); // Микрошаг
    tmcInterpolation.init(0);
    tmcStepperMaxSpeed.init(2000);
    tmcStepperTarget.init(10);
    vibroFr.init(10.0f);
    vibroAngle.init(10.0f);

    timber.i("DEBUG: DB init values - MaxSpeed: %d, Target: %d, VibroFr: %.2f, VibroAngle: %.2f",
             tmcStepperMaxSpeed.get(), tmcStepperTarget.get(),
             vibroFr.get(), vibroAngle.get());

    // Создаем таймер
    timer = timerBegin(10000); // Таймер 0, делитель 80 (80 МГц / 80 = 1 МГц)
    timerAttachInterrupt(timer, &onTimer);
    timerStart(timer);

    // eb.setEncISR(true);

    uint32_t drv_status = driver.DRV_STATUS();
    Serial.print("DRV_STATUS: 0x");
    Serial.println(drv_status, HEX);

    // driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160

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

    initTaskDb();

    // ble.init();

    // stepper.setMaxSpeed(1000000);
    // tmcStepperMaxSpeed.set(4000);
    // tmcStepperSetTarget.set(50);
    // stepper.setAcceleration(2000); // ускорение

    // TaskWifiLoop();
    // wifi_loop();


    //setupOTA();

}

void IRAM_ATTR vibro()
{
    //
    const float stepI = 1.8f / static_cast<float>(tmcDriverMicrostep.get()); // Угол поворота на один шаг
    //timber.i("DEBUG: stepI = %.6f, vibroAngle = %.2f", stepI, vibroAngle.get());
    const float a = (vibroAngle.get() / stepI);
    vibroTarget = static_cast<int>(a);
    //timber.i("DEBUG: a = %.6f, vibroTarget = %d", a, vibroTarget);
    //

    if (stepper.ready())
    {
        vibroDir = !vibroDir; // разворачиваем
        int32_t target = vibroDir * vibroTarget;
        //timber.i("DEBUG: vibroDir = %d, setting target = %d", vibroDir, target);
        stepper.setTarget(target); // едем в другую сторону
    }
}

void loop()
{

     // db.tick(); //тикер, вызывать в лупе

    if (currentMode.get() == WorkMode::CONTINUOUS)
    {
        // stepper.setSpeed(1000);
    }

    if (currentMode.get() == WorkMode::VIBRO)
    {
        vibro();
    }
    //    //Режим постоянный
    //    if (currentMode == WorkMode::CONTINUOUS) {
    //
    //        digitalWrite(STEP_PIN, HIGH);
    //        // // lay(1);
    //        delayMicroseconds(2000);
    //        digitalWrite(STEP_PIN, LOW);
    //        // // delay(1);
    //        delayMicroseconds(2000);
    //
    //    }
    //
    //    if (currentMode == WorkMode::VIBRO) {
    //
    //
    //    }

    //  eb.tick();
    //  // выбор переменной для изменения
    //   if (eb.hasClicks()) {
    //       select1 = eb.getClicks();
    //       timber.i.println(String("Select: ") + select1);
    //   }

    //   if (eb.turn()) {
    //       // меняем переменную
    //       switch (select1) {
    //           case 1:
    //               // изменение с шагом 5
    //               var1 += 5 * eb.dir();
    //               break;
    //           case 2:
    //               // изменение с шагом 1, при зажатой кнопке шаг 5
    //               var2 += (eb.pressing() ? 5 : 1) * eb.dir();
    //               break;
    //           case 3:
    //               // изменение с шагом 1, при быстром вращении шаг 5
    //               var3 += (eb.fast() ? 5 : 1) * eb.dir();
    //               break;
    //       }
    //       timber.i.println(String("vars ") + var1 + ',' + var2 + ',' + var3);
    //   }

    //}
    // shaft = !shaft;
    // driver.shaft(shaft);
}

TaskHandle_t TaskDb;

[[noreturn]] void TaskDbLoop(void *parameter)
{
    for (;;)
    {
        //db.tick();
        delay(1000);
    }
}

void initTaskDb()
{
    // xTaskCreatePinnedToCore(
    //     TaskDbLoop,   /* Функция для задачи */
    //     "TaskDbLoop", /* Имя задачи */
    //     20000,        /* Размер стека */
    //     nullptr,      /* Параметр задачи */
    //     5,            /* Приоритет */
    //     &TaskDb,      /* Выполняемая операция */
    //     0);           /* Номер ядра, на котором она должна выполняться */

}
