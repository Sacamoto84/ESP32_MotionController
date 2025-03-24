#include "observer.h"

void tmcStepperTargetObserver(){

    tmcStepperTarget.addObserver([](int32_t value) {
        timber.i("Observer: tmcStepperSetTarget изменен %d\n", value);
        stepper.setTarget(value);
        db.set(kk::_tmcStepperSetTarget, value);
    });

}