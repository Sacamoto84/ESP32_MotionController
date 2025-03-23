#include "observer.h"

void tmcDriverCurrentObserver(){

    //Установка тока
    tmcDriverCurrent.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverCurrent updated %d\n", value);
        driver.rms_current(value); // Set motor RMS current
        db.set(kk::_tmcDriverCurrent, value);
    });

}