#include "observer.h"

void tmcStepperTargetObserver(){

    tmcStepperTarget.addObserver([](int32_t value) {
        timber.i("Observer: tmcStepperSetTarget изменен %d", value);
        portENTER_CRITICAL(&stepperMux);
        stepper.setTarget(value);
        portEXIT_CRITICAL(&stepperMux);
        tmcStepperTarget.save();
    });

}