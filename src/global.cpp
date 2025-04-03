#include "global.h"

// Текущий режим
State<WorkMode> currentMode (WorkMode::CONTINUOUS);

State<int32_t> tmcStepperEnable(0);
State<int32_t> tmcDriverChop(0);
State<int32_t> tmcDriverCurrent(1);     //Ток двайвера
State<int32_t> tmcDriverMicrostep(512);     //Микрошаг
State<int32_t> tmcInterpolation(0);
State<int32_t> tmcStepperMaxSpeed(-1000);  // скорость движения к цели
State<int32_t> tmcStepperTarget(10);      // цель

//=== Cont ===
State <float> constRps(1); //Скорость оборотов в секунду



//=== Vibro ===
State<float>  vibroFr(10.0f);    //Частот вибрации Hz
State<float>  vibroAngle(30.0f); //Угол поворота 1..359
bool vibroDir = true; //Рантайм переменная направления
int32_t vibroTarget = 0.0f; //Рантайм переменная цели движения равен половине угла

GStepper2<STEPPER2WIRE> stepper(200, STEP_PIN, DIR_PIN, EN_PIN);

Timber timber;

extern GyverDBFile db;








