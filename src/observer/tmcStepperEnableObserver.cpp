#include "observer.h"

void tmcStepperEnableObserver(){

    tmcStepperEnable.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverEnable изменен %d\n", value);
        if (value) stepper.enable(); else stepper.disable();
        db.set(kk::_tmcDriverEnable, value);
    });

}