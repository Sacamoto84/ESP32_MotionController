#ifndef _GLOBAL_
#define _GLOBAL_

#include <Arduino.h>

#include "state.h"
#include "tmcGlobal.h"
#include "Timber.h"
#include "GyverStepper2.h"

extern GStepper2<STEPPER2WIRE> stepper;

enum class WorkMode {
    CONTINUOUS,
    VIBRO,
};

extern WorkMode currentMode;
extern timber Timber;


extern void observer();
extern void update();

extern State<uint16_t> tmcDriverEnable;
extern State<uint16_t> tmcDriverChop;
extern State<uint16_t> tmcDriverCurrent;
extern State<uint16_t> tmcDriverMicrostep; //Микрошаг
extern State<uint16_t> tmcDriverInterpolation;

extern State<uint16_t> tmcStepperMaxSpeed;
extern State<int32_t> tmcStepperSetTarget;   // цель

#endif // ends #ifndef _TFT_eSPIH_