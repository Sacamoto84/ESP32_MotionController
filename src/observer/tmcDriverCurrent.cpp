#include "observer.h"
#include "tmc2160Config.h"

void tmcDriverCurrentObserver(){

    //Установка тока
    tmcDriverCurrent.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverCurrent updated %d", value);
        applyTmc2160CurrentConfig();
        tmcDriverCurrent.save();
    });

}
