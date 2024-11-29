#ifndef _GLOBAL_
#define _GLOBAL_

#include <Arduino.h>

#include "state.h"
#include "tmcGlobal.h"
#include "Timber.h"

extern timber Timber;

extern void observer();

extern State<uint16_t> tmcDriverEnable;
extern State<uint16_t> tmcDriverChop;
extern State<uint16_t> tmcDriverCurrent;
extern State<uint16_t> tmcDriverMicrostep; //Микрошаг
extern State<uint16_t> tmcDriverInterpolation;

#endif // ends #ifndef _TFT_eSPIH_