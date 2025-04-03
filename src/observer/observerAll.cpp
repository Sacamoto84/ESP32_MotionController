#include "observer.h"

void observerAll(){


    tmcStepperTargetObserver();
    tmcStepperMaxSpeedObserver();
    tmcStepperEnableObserver();
    tmcDriverChopObserver();
    tmcDriverCurrentObserver();
    tmcDriverMicrostepObserver();
    vibroAngleObserver();
    vibroFrObserver();

    currentModeObserver();

    constRpsObserver();

}
