#include "global.h"

// Текущий режим
WorkMode currentMode = WorkMode::CONTINUOUS;

Timber timber;

State<int32_t> tmcStepperEnable(0);
State<int32_t> tmcDriverChop(0);
State<int32_t> tmcDriverCurrent(1000);     //Ток двайвера
State<int32_t> tmcDriverMicrostep(16);     //Микрошаг
State<int32_t> tmcInterpolation(0);

State<int32_t> tmcStepperMaxSpeed(1000); // скорость движения к цели
State<int32_t> tmcStepperTarget(10);   // цель

GStepper2<STEPPER2WIRE> stepper(200, STEP_PIN, DIR_PIN, EN_PIN);

#define EN_PIN 14 // Enable

extern GyverDBFile db;








