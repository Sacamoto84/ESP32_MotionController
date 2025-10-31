//
// Created by Ivan on 24.03.2025.
//
#include "observer.h"

void vibroFrObserver(){

    vibroFr.addObserver([](float value) {
        timber.i("Observer: vibroFr изменен %f", value);
        const float stepI = 1.8f/static_cast<float>(tmcDriverMicrostep.get()); //Угол поворота на один шаг
        const float a = (vibroAngle.get()/stepI)*2;
        stepper.setMaxSpeed(a*value);
        vibroFr.save();
     });

}