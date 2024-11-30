#include <Arduino.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <TMCStepper.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>

#include "global.h"

#include <EncButton.h>

extern EncButton eb;


extern void lcdInit();

extern void encoderInit();

#define DIAG0 35
#define DIAG1 34

extern void BLE_init();

extern void BLE_tick();

bool shaft = false;

// // esp8266/esp32
IRAM_ATTR void isr() {
    eb.tickISR();
}

#define FREQUENCY 100000 // Частота меандра в Гц (1 кГц)

hw_timer_t *timer = nullptr;

volatile bool state = false;

void IRAM_ATTR onTimer() {

    state = !state; // Переключаем состояние

    //Режим постоянный
//    if (currentMode == WorkMode::CONTINUOUS) {
//        digitalWrite(STEP_PIN, state);
//    }

    stepper.tick();

}

void setup() {

    // Создаем таймер
    timer = timerBegin(0, 80, true); // Таймер 0, делитель 80 (80 МГц / 80 = 1 МГц)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000 / (FREQUENCY * 2), true); // Половина периода
    timerAlarmEnable(timer);


    observer();

    lcdInit();

    encoderInit();
    //eb.setEncISR(true);

    pinMode(CS_PIN, INPUT);
    pinMode(SW_MOSI, INPUT);
    pinMode(SW_MISO, INPUT);
    pinMode(SW_SCK, INPUT);


    pinMode(DIAG0, INPUT);
    pinMode(DIAG1, INPUT);

    Serial.begin(921600);
    Serial2.begin(2000000);
    Serial2.printf("trtrtertert");


    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    tmcDriverEnable.set(true);

    driver.begin(); //  SPI: Init CS pins and possible SW SPI pins

    // UART: Init SW UART (if selected) with default 115200 baudrate
    driver.toff(5); // Enables driver in software

    //driver.rms_current(1200); // Set motor RMS current
    tmcDriverCurrent.set(700);
    tmcDriverMicrostep.set(0);
    //driver.microsteps(0); // Set microsteps to 1/16th


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


    BLE_init();

    stepper.setMaxSpeed(1000000);
    tmcStepperMaxSpeed.set(4000);
    tmcStepperSetTarget.set(50);
    //stepper.setAcceleration(2000); // ускорение

}

bool dir = 1;

void loop() {

    // здесь происходит движение моторов, вызывать как можно чаще

    //Режим постоянный
//    if (currentMode == WorkMode::CONTINUOUS) {
//        digitalWrite(STEP_PIN, state);
//    }




    //if (currentMode == WorkMode::VIBRO) {

    if (stepper.ready()) {
        dir = !dir;   // разворачиваем
        stepper.setTarget(dir * tmcStepperSetTarget.get()); // едем в другую сторону
    }

    //}


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











    // Run 5000 steps and switch direction in software
    // while (true) {

    // digitalWrite(STEP_PIN, HIGH);
    // // lay(1);
    // delayMicroseconds(2000);
    // digitalWrite(STEP_PIN, LOW);
    // // delay(1);
    // delayMicroseconds(2000);

    //}
    // shaft = !shaft;
    // driver.shaft(shaft);
}
