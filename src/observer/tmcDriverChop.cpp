#include "observer.h"

void tmcDriverChopObserver(){

    tmcDriverChop.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverChop updated");
        db.set(kk::_tmcDriverChop, value);
    });

}