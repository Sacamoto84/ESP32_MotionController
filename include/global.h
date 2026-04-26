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

extern State<int32_t> constDirection;
extern State<float> constRps; // Скорость оборотов в секунду

extern State<int32_t> tmcFastStandstill;
extern State<int32_t> tmcTPwmThrs;
extern State<int32_t> tmcTPowerDown;
extern State<int32_t> tmcIrun;
extern State<int32_t> tmcIhold;
extern State<int32_t> tmcIholdDelay;
extern State<int32_t> tmcGlobalScaler;
extern State<int32_t> tmcFreewheel;
extern State<int32_t> tmcToff;
extern State<int32_t> tmcTbl;
extern State<int32_t> tmcHstrt;
extern State<int32_t> tmcHend;
extern State<int32_t> tmcChm;
extern State<int32_t> tmcDedge;
extern State<int32_t> tmcVhighfs;
extern State<int32_t> tmcVhighchm;
extern State<int32_t> tmcPwmAutoscale;
extern State<int32_t> tmcPwmAutograd;
extern State<int32_t> tmcPwmOfs;
extern State<int32_t> tmcPwmGrad;
extern State<int32_t> tmcPwmFreq;
extern State<int32_t> tmcPwmReg;
extern State<int32_t> tmcPwmLim;
extern State<int32_t> tmcSgt;
extern State<int32_t> tmcSfilt;
extern State<int32_t> tmcTCoolThrs;
extern State<int32_t> tmcTHigh;
extern State<int32_t> tmcSemin;
extern State<int32_t> tmcSemax;
extern State<int32_t> tmcSeup;
extern State<int32_t> tmcSedn;
extern State<int32_t> tmcSeimin;
extern State<int32_t> tmcVdcMin;
extern State<int32_t> tmcDcTime;
extern State<int32_t> tmcDcSg;
extern State<int32_t> tmcRecalibrate;
extern State<int32_t> tmcMultistepFilt;
extern State<int32_t> tmcStopEnable;
extern State<int32_t> tmcDiag0Error;
extern State<int32_t> tmcDiag0Otw;
extern State<int32_t> tmcDiag0Stall;
extern State<int32_t> tmcDiag1Stall;
extern State<int32_t> tmcDiag1Index;
extern State<int32_t> tmcDiag1Onstate;
extern State<int32_t> tmcDiag1StepsSkipped;
extern State<int32_t> tmcDiag0PushPull;
extern State<int32_t> tmcDiag1PushPull;
extern State<int32_t> tmcS2vsLevel;
extern State<int32_t> tmcS2gLevel;
extern State<int32_t> tmcShortFilter;
extern State<int32_t> tmcShortDelay;
extern State<int32_t> tmcBbmTime;
extern State<int32_t> tmcBbmClks;
extern State<int32_t> tmcOtSelect;
extern State<int32_t> tmcDrvStrength;
extern State<int32_t> tmcFiltIsense;

#endif // ends #ifndef _TFT_eSPIH_
