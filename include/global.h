#ifndef _GLOBAL_
#define _GLOBAL_

#include <Arduino.h>

#include "state.h"
#include "tmcGlobal.h"
#include "Timber.h"
#include "GyverStepper2.h"
#include <GyverDB.h>

#include <LittleFS.h>

#include "GyverDBFile.h"


// имена ячеек базы данных
DB_KEYS(kk,
    _tmcDriverEnable,
    _tmcDriverChop,
    _tmcDriverCurrent,
    _tmcDriverMicrostep,
    _tmcDriverInterpolation,
    _tmcStepperMaxSpeed,
    _tmcStepperSetTarget
);

extern GStepper2<STEPPER2WIRE> stepper;

enum class WorkMode {
    CONTINUOUS,
    VIBRO,
};

extern WorkMode currentMode;

extern Timber timber;


extern void observerAll();
extern void update();

extern State<int32_t> tmcStepperEnable;
extern State<int32_t> tmcDriverChop;
extern State<int32_t> tmcDriverCurrent;
extern State<int32_t> tmcDriverMicrostep; //Микрошаг
extern State<int32_t> tmcInterpolation;

extern State<int32_t> tmcStepperMaxSpeed;
extern State<int32_t> tmcStepperTarget;   // цель
extern State<float>  vibroFr;    //Частот вибрации Hz
extern State<float>  vibroAngle; //Угол поворота 1..359
extern bool vibroDir; //Рантайм переменная направления

#endif // ends #ifndef _TFT_eSPIH_