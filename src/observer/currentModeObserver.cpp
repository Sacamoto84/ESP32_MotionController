#include "observer.h"

void currentModeObserver()
{
    currentMode.addObserver([](WorkMode value) {
        timber.i("Observer: currentMode updated");

        if (value == WorkMode::VIBRO)
        {
            portENTER_CRITICAL(&stepperMux);
            stepper.brake();
            stepper.setCurrent(0);
            portEXIT_CRITICAL(&stepperMux);

            vibroDir.store(false);
            vibroAngle.notifyObservers();
            return;
        }

        tmcStepperMaxSpeed.notifyObservers();
        tmcStepperTarget.notifyObservers();
    });
}
