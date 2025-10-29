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

#include <JC_EEPROM.h> // https://github.com/JChristensen/JC_EEPROM
// #include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
#include <Wire.h> // https://arduino.cc/en/Reference/Wire

// имена ячеек базы данных
DB_KEYS(kk,
        _tmcDriverEnable,
        _tmcDriverChop,
        _tmcDriverCurrent,
        _tmcDriverMicrostep,
        _tmcDriverInterpolation,
        _tmcStepperMaxSpeed,
        _tmcStepperSetTarget,
        _vibroAngle,
        _vibroFr);

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

// Базовые типы
#define EEPROM_SIZE_BOOL 1
#define EEPROM_SIZE_INT 2
#define EEPROM_SIZE_LONG 4
#define EEPROM_SIZE_FLOAT 4
#define EEPROM_SIZE_DOUBLE 8
#define EEPROM_SIZE_INT32   4
#define EEPROM_SIZE_FLOAT   4

#define EEPROM_TMC_DRIVER_ENABLE        0x00    // int32_t  (4 байта)
#define EEPROM_TMC_DRIVER_CHOP          0x04    // int32_t  (4 байта)
#define EEPROM_TMC_DRIVER_CURRENT       0x08    // int32_t  (4 байта)
#define EEPROM_TMC_DRIVER_MICROSTEP     0x0C    // int32_t  (4 байта)  ← 0x08 + 4
#define EEPROM_TMC_DRIVER_INTERPOLATION 0x10    // int32_t  (4 байта)  ← 0x0C + 4
#define EEPROM_TMC_STEPPER_MAXSPEED     0x14    // int32_t  (4 байта)  ← 0x10 + 4
#define EEPROM_TMC_STEPPER_SETTARGET    0x18    // int32_t  (4 байта)  ← 0x14 + 4
#define EEPROM_VIBRO_ANGLE              0x1C    // float    (4 байта)  ← 0x18 + 4
#define EEPROM_VIBRO_FREC               0x20    // float    (4 байта)  ← 0x1C + 4

// ============================================================================
// ИТОГО: 0x24 байта (36 байт) используется
// ============================================================================
#define EEPROM_TMC_VIBRO_TOTAL_SIZE     0x24

#endif // ends #ifndef _TFT_eSPIH_