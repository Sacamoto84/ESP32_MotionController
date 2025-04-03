//
// Created by Ivan on 28.03.2025.
//
#include "observer.h"

void currentModeObserver(){

    currentMode.addObserver([](WorkMode value) {

         timber.i("Observer: currentMode изменен");


         //db.set(kk::_vibroAngle, value);

     });

}