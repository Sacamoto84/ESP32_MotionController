//
// Created by Ivan on 26.03.2025.
//
#include "observer.h"

void vibroAngleObserver(){

    vibroAngle.addObserver([](float value) {
         timber.i("Observer: vibroAngle изменен %f", value);
         db.set(kk::_vibroAngle, value);
     });

}