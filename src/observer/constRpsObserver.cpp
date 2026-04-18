#include "observer.h"

void constRpsObserver(){

    constRps.addObserver([](float value) {
         timber.i("Observer: constRps изменен %f", value);
         portENTER_CRITICAL(&stepperMux);
         stepper.setSpeedDeg(360 * value);
         portEXIT_CRITICAL(&stepperMux);
         //db.set(kk::_vibroAngle, value);
     });

}