#include "observer.h"

void observerAll(){
    tmcStepperSetTargetObserver();
    tmcStepperMaxSpeedObserver();
    tmcDriverEnableObserver();
    tmcDriverChopObserver();
    tmcDriverCurrentObserver();
    tmcDriverMicrostepObserver();
}
