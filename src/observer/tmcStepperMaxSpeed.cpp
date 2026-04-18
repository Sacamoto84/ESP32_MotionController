#include "observer.h"

void tmcStepperMaxSpeedObserver(){

    tmcStepperMaxSpeed.addObserver([](int32_t value) {
        timber.i("Observer: tmcStepperMaxSpeed изменен %d", value);
        portENTER_CRITICAL(&stepperMux);
        stepper.setMaxSpeed(value);
        portEXIT_CRITICAL(&stepperMux);
        tmcStepperMaxSpeed.save();
    });

}