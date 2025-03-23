//
// Created by Ivan on 23.03.2025.
//

#ifndef OBSERVER_H
#define OBSERVER_H

#include "global.h"

extern GyverDBFile db;

extern void tmcStepperSetTargetObserver();
extern void tmcStepperMaxSpeedObserver();
extern void tmcDriverEnableObserver();
extern void tmcDriverChopObserver();
extern void tmcDriverCurrentObserver();
extern void tmcDriverMicrostepObserver();

#endif //OBSERVER_H
