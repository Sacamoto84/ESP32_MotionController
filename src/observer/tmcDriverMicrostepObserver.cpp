#include "observer.h"

void tmcDriverMicrostepObserver(){
    //Установка микрошага
    tmcDriverMicrostep.addObserver([](int32_t value) {
        timber.i("Observer: tmcDriverMicrostep изменен %d", value);
        uint16_t a;
        if (value == 1) a = 0; else a = value;
        driver.microsteps(a);
        db.set(kk::_tmcDriverMicrostep, value);

        if (currentMode.get() == WorkMode::VIBRO)
        {
            vibroAngle.notifyObservers();
            vibroFr.notifyObservers();
        }

    });

}