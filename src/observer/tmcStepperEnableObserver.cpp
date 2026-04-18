#include "observer.h"

void tmcStepperEnableObserver(){

    tmcStepperEnable.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverEnable изменен %d", value);
        if (value) {
            portENTER_CRITICAL(&stepperMux);
            stepper.enable();
            portEXIT_CRITICAL(&stepperMux);
        } else {
            portENTER_CRITICAL(&stepperMux);
            stepper.disable();
            portEXIT_CRITICAL(&stepperMux);
        }
        tmcStepperEnable.save();
    });

}