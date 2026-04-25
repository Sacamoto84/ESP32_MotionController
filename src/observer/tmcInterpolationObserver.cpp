#include "observer.h"

void tmcInterpolationObserver() {

    tmcInterpolation.addObserver([](int32_t value) {
        timber.i("Observer: tmcInterpolation updated %d", value);
        driver.intpol(value != 0);
        tmcInterpolation.save();
    });

}
