#include "observer.h"

void constRpsObserver(){

    constRps.addObserver([](float value) {
         timber.i("Observer: constRps изменен %f", value);
         stepper.setSpeedDeg(360 * value);
         //db.set(kk::_vibroAngle, value);
     });

}