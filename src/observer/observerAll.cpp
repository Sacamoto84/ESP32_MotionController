#include "observer.h"

void observerAll(){
    tmcStepperTargetObserver();
    tmcStepperMaxSpeedObserver();
    tmcStepperEnableObserver();
    tmcDriverChopObserver();
    tmcDriverCurrentObserver();
    tmcDriverMicrostepObserver();
    vibroFrObserver();
    vibroAngleObserver();
}
