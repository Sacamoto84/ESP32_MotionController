//
// Created by Ivan on 23.03.2025.
//

#ifndef OBSERVER_H
#define OBSERVER_H

#include "global.h"

extern GyverDBFile db;

extern void tmcStepperTargetObserver();
extern void tmcStepperMaxSpeedObserver();
extern void tmcStepperEnableObserver();
extern void tmcDriverChopObserver();
extern void tmcDriverCurrentObserver();
extern void tmcDriverMicrostepObserver();
extern void vibroFrObserver();
extern void vibroAngleObserver();

extern void currentModeObserver();
extern void constRpsObserver();

#endif //OBSERVER_H
