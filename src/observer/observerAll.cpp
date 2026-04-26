#include "observer.h"

void observerAll(){
    
    tmcStepperTargetObserver();
    tmcStepperMaxSpeedObserver();
    tmcStepperEnableObserver();
    tmcDriverChopObserver();
    tmcDriverCurrentObserver();
    tmcDriverMicrostepObserver();
    tmcInterpolationObserver();
    tmc2160SettingsObserver();
    vibroAngleObserver();
    vibroFrObserver();

    currentModeObserver();
    constDirectionObserver();
    constRpsObserver();
    
}
