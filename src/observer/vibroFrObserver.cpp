#include "observer.h"

void vibroFrObserver()
{
    vibroFr.addObserver([](float value) {
        timber.i("Observer: vibroFr updated %f", value);

        if (currentMode.get() == WorkMode::VIBRO)
        {
            const float stepI = 1.8f / static_cast<float>(tmcDriverMicrostep.get());
            const float a = (vibroAngle.get() / stepI) * 2;
            portENTER_CRITICAL(&stepperMux);
            stepper.setMaxSpeed(a * value);
            portEXIT_CRITICAL(&stepperMux);
        }

        vibroFr.save();
    });
}
