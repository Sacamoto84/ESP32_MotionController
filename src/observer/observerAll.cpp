#include "observer.h"

void observerAll(){
    
    tmcStepperTargetObserver();
    tmcStepperMaxSpeedObserver();
    tmcStepperEnableObserver();
    tmcDriverChopObserver();
    tmcDriverCurrentObserver();
    tmcDriverMicrostepObserver();
    tmcInterpolationObserver();
    vibroAngleObserver();
    vibroFrObserver();

    currentModeObserver();
    constRpsObserver();
    
}
