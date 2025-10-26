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

extern EncButton eb;

extern void lcdInit();

extern void encoderInit();

extern void wifi_loop();

extern AsyncWebSocket ws;

//Ble ble;

extern void initTaskDb();

bool shaft = false;

GyverDBFile db;

//esp8266/esp32
// IRAM_ATTR void isr() {
//     eb.tickISR();
// }

#define FREQUENCY 100000 // Частота меандра в Гц (1 кГц)

hw_timer_t *timer = nullptr;

//volatile bool state = false;

bool dir = 1;

void IRAM_ATTR onTimer() {
    //state = !state; // Переключаем состояние
    stepper.tick();
}

void setup() {
    esp_task_wdt_init(30, false);
    Serial.begin(921600);
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

    // запуск и инициализация полей БД
    const bool res = db.begin();

    if (!res)
        timber.e("Ошибка открытия базы");

    else timber.i("База на диске");
    db.dump(Serial2);
    timber.i("--------------------------------");

    db.init(kk::_tmcDriverEnable, 0);
    db.init(kk::_tmcDriverChop, 0);
    db.init(kk::_tmcDriverCurrent, 1000);
    db.init(kk::_tmcDriverMicrostep, 16);  //Микрошаг
    db.init(kk::_tmcDriverInterpolation, 0);
    db.init(kk::_tmcStepperMaxSpeed, 2000);
    db.init(kk::_tmcStepperSetTarget, 10);
    db.init(kk::_vibroFr, 10.0f);
    db.init(kk::_vibroAngle, 10.0f);

    observerAll();

    tmcStepperEnable.set(db.get(kk::_tmcDriverEnable));
    tmcDriverChop.set(db.get(kk::_tmcDriverChop));
    tmcDriverCurrent.set(db.get(kk::_tmcDriverCurrent));
    tmcDriverMicrostep.set(db.get(kk::_tmcDriverMicrostep));
    tmcInterpolation.set(db.get(kk::_tmcDriverInterpolation));
    tmcStepperMaxSpeed.set(db.get(kk::_tmcStepperMaxSpeed));
    tmcStepperTarget.set(db.get(kk::_tmcStepperSetTarget));

    vibroAngle.set(db.get(kk::_vibroAngle));
    vibroFr.set(db.get(kk::_vibroFr));


    // Создаем таймер
    timer = timerBegin(0, 80, true); // Таймер 0, делитель 80 (80 МГц / 80 = 1 МГц)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000 / (FREQUENCY * 2), true); // Половина периода
    timerAlarmEnable(timer);

    lcdInit();


    //eb.setEncISR(true);

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

    //ble.init();

    //stepper.setMaxSpeed(1000000);
    //tmcStepperMaxSpeed.set(4000);
    //tmcStepperSetTarget.set(50);
    //stepper.setAcceleration(2000); // ускорение

    //TaskWifiLoop();
    //wifi_loop();



}



void IRAM_ATTR vibro()
{
        //
        const float stepI = 1.8f/static_cast<float>(tmcDriverMicrostep.get()); //Угол поворота на один шаг
        const float a = (vibroAngle.get()/stepI);
        vibroTarget = static_cast<int32_t>(a);
        //

        if (stepper.ready()) {
            vibroDir = !vibroDir;   // разворачиваем
            stepper.setTarget(vibroDir * vibroTarget); // едем в другую сторону
        }
}


void loop() {

    // Обработка команд CLI
    if (Serial2.available()) {
        String input = Serial2.readStringUntil('\n');
        cli.parse(input);
    }

    //db.tick(); //тикер, вызывать в лупе

    if (currentMode.get()  == WorkMode::CONTINUOUS)
    {
        //stepper.setSpeed(1000);
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
              TaskDbLoop,    /* Функция для задачи */
              "TaskDbLoop",     /* Имя задачи */
              10000,         /* Размер стека */
              nullptr,          /* Параметр задачи */
              5,                /* Приоритет */
              &TaskDb,                  /* Выполняемая операция */
              0);                /* Номер ядра, на котором она должна выполняться */
}