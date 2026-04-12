#include "observer.h"

void tmcStepperTargetObserver(){

    tmcStepperTarget.addObserver([](int32_t value) {
        timber.i("Observer: tmcStepperSetTarget изменен %d", value);
        stepper.setTarget(value);
        tmcStepperTarget.save();
    });

}