//
// Created by Ivan on 24.03.2025.
//
#include "observer.h"

void vibroFrObserver(){

    vibroFr.addObserver([](float value) {
         timber.i("Observer: vibroFr изменен %f", value);
         stepper.setMaxSpeed(3000);
         db.set(kk::_vibroFr, value);
     });

}