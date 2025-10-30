#ifndef _GLOBAL_
#define _GLOBAL_

#include <Arduino.h>

#include "state.h"
#include "tmcGlobal.h"
#include "Timber.h"
#include "GyverStepper2.h"

#include <LittleFS.h>

#include <JC_EEPROM.h> // https://github.com/JChristensen/JC_EEPROM
// #include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
#include <Wire.h> // https://arduino.cc/en/Reference/Wire

#include "addresses.h"

extern GStepper2<STEPPER2WIRE> stepper;

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
extern bool vibroDir;           // Рантайм переменная направления
extern int vibroTarget;

extern State<float> constRps; // Скорость оборотов в секунду

extern JC_EEPROM eep;

extern void eepromDump(uint32_t, uint32_t);

#endif // ends #ifndef _TFT_eSPIH_