#ifndef _GLOBAL_
#define _GLOBAL_

#include <Arduino.h>
#include <atomic>

#include "state.h"
#include "tmcGlobal.h"
#include "Timber.h"
#include "GyverStepper2.h"

#include <LittleFS.h>

#include "addresses.h"

extern GStepper2<STEPPER2WIRE> stepper;
extern portMUX_TYPE stepperMux;

enum class WorkMode
{
    CONTINUOUS,
    VIBRO,
};

// extern WorkMode currentMode;

extern State<WorkMode> currentMode;

extern Timber timber;

extern void observerAll();
extern void update();

extern State<int32_t> tmcStepperEnable;
extern State<int32_t> tmcDriverChop;
extern State<int32_t> tmcDriverCurrent;
extern State<int32_t> tmcDriverMicrostep; // Микрошаг
extern State<int32_t> tmcInterpolation;

extern State<int32_t> tmcStepperMaxSpeed;
extern State<int32_t> tmcStepperTarget; // цель

extern State<float> vibroFr;    // Частот вибрации Hz
extern State<float> vibroAngle; // Угол поворота 1..359
extern std::atomic<bool> vibroDir;
extern std::atomic<int> vibroTarget;

extern State<float> constRps; // Скорость оборотов в секунду

#endif // ends #ifndef _TFT_eSPIH_