#include "global.h"

GyverDBFile db(&LittleFS, "data.db");

// Текущий режим
WorkMode currentMode = WorkMode::CONTINUOUS;

Timber timber;

State<int32_t> tmcDriverEnable(0);
State<int32_t> tmcDriverChop(0);
State<int32_t> tmcDriverCurrent(1000);     //Ток двайвера
State<int32_t> tmcDriverMicrostep(16);     //Микрошаг
State<int32_t> tmcDriverInterpolation(0);

State<int32_t> tmcStepperMaxSpeed(1000); // скорость движения к цели
State<int32_t> tmcStepperSetTarget(10);   // цель

GStepper2<STEPPER2WIRE> stepper(200, STEP_PIN, DIR_PIN, EN_PIN);

#define EN_PIN 14 // Enable

void observer() {

    tmcStepperSetTarget.addObserver([](int32_t value) {
        Serial2.printf("Observer: tmcStepperSetTarget изменен %d\n", value);
        stepper.setTarget(value);
    });


    tmcStepperMaxSpeed.addObserver([](int32_t value) {
        Serial2.printf("Observer: tmcStepperMaxSpeed изменен %d\n", value);
        stepper.setMaxSpeed(value);
        //stepper.setSpeed(value);
    });


    tmcDriverEnable.addObserver([](int32_t value) {
        Serial2.printf("Observer: tmcDriverEnable изменен %d\n", value);
        if (value)
            stepper.enable();
        else
            stepper.disable();
    });


    tmcDriverChop.addObserver([](int32_t value) {
        Serial2.println("Observer: tmcDriverChop updated");

    });

    //Установка тока
    tmcDriverCurrent.addObserver([](int32_t value) {
        Serial2.printf("Observer: tmcDriverCurrent updated %d\n", value);
        driver.rms_current(value); // Set motor RMS current
    });

    //Установка микрошага
    tmcDriverMicrostep.addObserver([](int32_t value) {
        Serial2.printf("Observer: tmcDriverMicrostep изменен %d\n", value);
        uint16_t a;
        if (value == 1) a = 0; else a = value;
        driver.microsteps(a);
    });



}





