//
// Created by Ivan on 24.03.2025.
//
#include "observer.h"

void vibroFrObserver(){

    vibroFr.addObserver([](float value) {
         timber.i("Observer: vibroFr изменен %f\n", value);

         //db.set(kk::_tmcStepperSetTarget, value);
     });

}