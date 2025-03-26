#include "observer.h"

void tmcStepperMaxSpeedObserver(){

    tmcStepperMaxSpeed.addObserver([](int32_t value) {
        timber.i("Observer: tmcStepperMaxSpeed изменен %d", value);
        stepper.setMaxSpeed(value);
        db.set(kk::_tmcStepperMaxSpeed, value);
    });

}