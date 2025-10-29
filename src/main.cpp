#include <Arduino.h>

#include "FS.h"
#include "LITTLEFS.h"

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <TMCStepper.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>

#include <esp_task_wdt.h>

#include "global.h"

#include <EncButton.h>
#include <ble/ble.h>

#include <ESPAsyncWebServer.h>

#include <JC_EEPROM.h> // https://github.com/JChristensen/JC_EEPROM
// #include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
#include <Wire.h> // https://arduino.cc/en/Reference/Wire

constexpr bool erase{false};        // true writes 0xFF to all addresses,
                                    //   false performs write/read test
constexpr uint32_t totalKBytes{64}; // for read and write test functions
constexpr uint8_t btnStart{6};      // pin for start button

extern EncButton eb;

extern void lcdInit();

extern void encoderInit();

extern void wifi_loop();

extern AsyncWebSocket ws;

// Ble ble;

extern void initTaskDb();

bool shaft = false;

GyverDBFile db;

// esp8266/esp32
//  IRAM_ATTR void isr() {
//      eb.tickISR();
//  }

#define FREQUENCY 100000 // Частота меандра в Гц (1 кГц)

hw_timer_t *timer = nullptr;

// volatile bool state = false;

bool dir = 1;

void IRAM_ATTR onTimer()
{
    // state = !state; // Переключаем состояние

    // static uint32_t tickCount = 0;
    // tickCount++;
    // if (tickCount % 1000 == 0)
    // {
    //     timber.i("DEBUG: onTimer tick count = %d", tickCount);
    // }
    // stepper.tick();
}

void setup()
{
    esp_task_wdt_init(30, false);
    Serial.begin(115200);
    Serial2.begin(2000000);

    Timber::clear();
    timber.i("--------------------------------");
    timber.i("Контроллер шагового мотора V1.0");
    timber.w("Ворнинг");
    timber.e("Ошибка");
    timber.colorStringln(15, 36, "Проверка 15 36");
    timber.i("--------------------------------");

    encoderInit();
    tmcInit();
    cliInit();

    if (!LittleFS.begin(true))
        timber.e("Ошибка при монтировании LittleFS");

    db.setFS(&LittleFS, "/data.db");

    uint8_t eepStatus = eep.begin(JC_EEPROM::twiClock100kHz); // go fast!

    // 0: success.
    // 1: data too long to fit in transmit buffer.
    // 2: received NACK on transmit of address.
    // 3: received NACK on transmit of data.
    // 4: other error.
    // 5: timeout

    switch (eepStatus)
    {
    case 0:
        timber.s("extEEPROM.begin() success");
        break;

    case 1:
        timber.e("extEEPROM.begin() failed, 1: data too long to fit in transmit buffer.");
        break;

    case 2:
        timber.e("extEEPROM.begin() failed, 2: received NACK on transmit of address.");
        break;

    case 3:
        timber.e("extEEPROM.begin() failed, 3: received NACK on transmit of data.");
        break;

    case 4:
        timber.e("extEEPROM.begin() failed, 4: other error.");
        break;

    case 5:
        timber.e("extEEPROM.begin() failed, 5 :timeout.");
        break;

    default:
        timber.e("extEEPROM.begin() failed, default");
        break;
    }

    eepromDump(0, 32);
    eep.writeInt(0, 0x1234);
    eep.writeFloat(2, 1.2F);
    eepromDump(0, 32);
    int temp = eep.readInt(0);
    timber.println("read Int = 0x%x", temp);
    timber.println("read Float = 0x%f", eep.readFloat(2));

    // eep.write(0, 10);

    // запуск и инициализация полей БД
    const bool res = db.begin();

    if (!res)
        timber.e("Ошибка открытия базы");

    else
        timber.i("База на диске");
    db.dump(Serial2);
    timber.i("--------------------------------");
    
    db.init(kk::_tmcDriverEnable, 0);
    db.init(kk::_tmcDriverChop, 0);
    db.init(kk::_tmcDriverCurrent, 1000);
    db.init(kk::_tmcDriverMicrostep, 16); // Микрошаг
    db.init(kk::_tmcDriverInterpolation, 0);
    db.init(kk::_tmcStepperMaxSpeed, 2000);
    db.init(kk::_tmcStepperSetTarget, 10);
    db.init(kk::_vibroFr, 10.0f);
    db.init(kk::_vibroAngle, 10.0f);

    timber.i("DEBUG: DB init values - MaxSpeed: %d, Target: %d, VibroFr: %.2f, VibroAngle: %.2f",
             db.get(kk::_tmcStepperMaxSpeed), db.get(kk::_tmcStepperSetTarget),
             db.get(kk::_vibroFr), db.get(kk::_vibroAngle));

    observerAll();

    tmcDriverChop.set(db.get(kk::_tmcDriverChop));
    tmcDriverMicrostep.set(db.get(kk::_tmcDriverMicrostep));
    tmcInterpolation.set(db.get(kk::_tmcDriverInterpolation));
    tmcStepperMaxSpeed.set(db.get(kk::_tmcStepperMaxSpeed));
    tmcStepperTarget.set(db.get(kk::_tmcStepperSetTarget));
    vibroAngle.set(db.get(kk::_vibroAngle));
    vibroFr.set(db.get(kk::_vibroFr));
    tmcDriverCurrent.set(db.get(kk::_tmcDriverCurrent));
    tmcStepperEnable.set(db.get(kk::_tmcDriverEnable));

    // Создаем таймер
    timer = timerBegin(0, 80, true); // Таймер 0, делитель 80 (80 МГц / 80 = 1 МГц)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000 / (FREQUENCY * 2), true); // Половина периода
    timerAlarmEnable(timer);

    lcdInit();

    // eb.setEncISR(true);

    uint32_t drv_status = driver.DRV_STATUS();
    Serial2.print("DRV_STATUS: 0x");
    Serial2.println(drv_status, HEX);

    // driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160

    Serial2.print("DRV_STATUS=0b");
    Serial2.println(driver.DRV_STATUS(), BIN);

    Serial2.print("version: ");
    Serial2.println(driver.version());

    Serial2.print("run status: ");
    Serial2.println(driver.irun());

    Serial2.print("run en stat: ");
    Serial2.println(driver.encb_dcen_cfg4());

    Serial2.print("toff: ");
    Serial2.println(driver.toff());

    initTaskDb();

    // ble.init();

    // stepper.setMaxSpeed(1000000);
    // tmcStepperMaxSpeed.set(4000);
    // tmcStepperSetTarget.set(50);
    // stepper.setAcceleration(2000); // ускорение

    // TaskWifiLoop();
    // wifi_loop();
}

void IRAM_ATTR vibro()
{
    //
    const float stepI = 1.8f / static_cast<float>(tmcDriverMicrostep.get()); // Угол поворота на один шаг
    timber.i("DEBUG: stepI = %.6f, vibroAngle = %.2f", stepI, vibroAngle.get());
    const float a = (vibroAngle.get() / stepI);
    vibroTarget = static_cast<int32_t>(a);
    timber.i("DEBUG: a = %.6f, vibroTarget = %d", a, vibroTarget);
    //

    if (stepper.ready())
    {
        vibroDir = !vibroDir; // разворачиваем
        int32_t target = vibroDir * vibroTarget;
        timber.i("DEBUG: vibroDir = %d, setting target = %d", vibroDir, target);
        stepper.setTarget(target); // едем в другую сторону
    }
}

void loop()
{

    // Обработка команд CLI
    if (Serial2.available())
    {
        String input = Serial2.readStringUntil('\n');
        cli.parse(input);
    }

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
    //       Serial2.println(String("Select: ") + select1);
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
    //       Serial2.println(String("vars ") + var1 + ',' + var2 + ',' + var3);
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
        db.tick();
        delay(1000);
    }
}

void initTaskDb()
{
    xTaskCreatePinnedToCore(
        TaskDbLoop,   /* Функция для задачи */
        "TaskDbLoop", /* Имя задачи */
        20000,        /* Размер стека */
        nullptr,      /* Параметр задачи */
        5,            /* Приоритет */
        &TaskDb,      /* Выполняемая операция */
        0);           /* Номер ядра, на котором она должна выполняться */
}