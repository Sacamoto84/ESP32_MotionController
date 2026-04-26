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
State<int32_t> constDirection(1);
State<float> constRps(1); // Скорость оборотов в секунду

State<int32_t> tmcFastStandstill(0);
State<int32_t> tmcTPwmThrs(0);
State<int32_t> tmcTPowerDown(10);
State<int32_t> tmcIrun(31);
State<int32_t> tmcIhold(16);
State<int32_t> tmcIholdDelay(6);
State<int32_t> tmcGlobalScaler(0);
State<int32_t> tmcFreewheel(0);
State<int32_t> tmcToff(8);
State<int32_t> tmcTbl(1);
State<int32_t> tmcHstrt(5);
State<int32_t> tmcHend(2);
State<int32_t> tmcChm(0);
State<int32_t> tmcDedge(0);
State<int32_t> tmcVhighfs(0);
State<int32_t> tmcVhighchm(0);
State<int32_t> tmcPwmAutoscale(1);
State<int32_t> tmcPwmAutograd(1);
State<int32_t> tmcPwmOfs(30);
State<int32_t> tmcPwmGrad(0);
State<int32_t> tmcPwmFreq(0);
State<int32_t> tmcPwmReg(4);
State<int32_t> tmcPwmLim(12);
State<int32_t> tmcSgt(0);
State<int32_t> tmcSfilt(0);
State<int32_t> tmcTCoolThrs(0);
State<int32_t> tmcTHigh(0);
State<int32_t> tmcSemin(0);
State<int32_t> tmcSemax(0);
State<int32_t> tmcSeup(0);
State<int32_t> tmcSedn(0);
State<int32_t> tmcSeimin(0);
State<int32_t> tmcVdcMin(0);
State<int32_t> tmcDcTime(0);
State<int32_t> tmcDcSg(0);
State<int32_t> tmcRecalibrate(0);
State<int32_t> tmcMultistepFilt(0);
State<int32_t> tmcStopEnable(0);
State<int32_t> tmcDiag0Error(0);
State<int32_t> tmcDiag0Otw(0);
State<int32_t> tmcDiag0Stall(0);
State<int32_t> tmcDiag1Stall(0);
State<int32_t> tmcDiag1Index(0);
State<int32_t> tmcDiag1Onstate(0);
State<int32_t> tmcDiag1StepsSkipped(0);
State<int32_t> tmcDiag0PushPull(0);
State<int32_t> tmcDiag1PushPull(0);
State<int32_t> tmcS2vsLevel(6);
State<int32_t> tmcS2gLevel(6);
State<int32_t> tmcShortFilter(1);
State<int32_t> tmcShortDelay(0);
State<int32_t> tmcBbmTime(0);
State<int32_t> tmcBbmClks(4);
State<int32_t> tmcOtSelect(0);
State<int32_t> tmcDrvStrength(2);
State<int32_t> tmcFiltIsense(0);

//=== Vibro ===
State<float> vibroFr(10.0f, EEPROM_VIBRO_FREC);     // Частот вибрации Hz
State<float> vibroAngle(30.0f, EEPROM_VIBRO_ANGLE); // Угол поворота 1..359
std::atomic<bool> vibroDir{true};
std::atomic<int> vibroTarget{0};

GStepper2<STEPPER2WIRE> stepper(200, STEP_PIN, DIR_PIN, EN_PIN);
portMUX_TYPE stepperMux = portMUX_INITIALIZER_UNLOCKED;

Timber timber;
