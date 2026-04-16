#include "global.h"

// Текущий режим
State<WorkMode> currentMode(WorkMode::CONTINUOUS);

State<int32_t> tmcStepperEnable(0, EEPROM_TMC_DRIVER_ENABLE);
State<int32_t> tmcDriverChop(0, EEPROM_TMC_DRIVER_CHOP);
State<int32_t> tmcDriverCurrent(1, EEPROM_TMC_DRIVER_CURRENT);      // Ток двайвера
State<int32_t> tmcDriverMicrostep(16, EEPROM_TMC_DRIVER_MICROSTEP); // Микрошаг
State<int32_t> tmcInterpolation(0, EEPROM_TMC_DRIVER_INTERPOLATION);
State<int32_t> tmcStepperMaxSpeed(1000, EEPROM_TMC_STEPPER_MAXSPEED); // скорость движения к цели
State<int32_t> tmcStepperTarget(10, EEPROM_TMC_STEPPER_SETTARGET);    // цель

//=== Cont ===
State<float> constRps(1); // Скорость оборотов в секунду

//=== Vibro ===
State<float> vibroFr(10.0f, EEPROM_VIBRO_FREC);     // Частот вибрации Hz
State<float> vibroAngle(30.0f, EEPROM_VIBRO_ANGLE); // Угол поворота 1..359
bool vibroDir = true;                               // Рантайм переменная направления
int vibroTarget = 0.0f;                         // Рантайм переменная цели движения равен половине угла

GStepper2<STEPPER2WIRE> stepper(200, STEP_PIN, DIR_PIN, EN_PIN);

Timber timber;
