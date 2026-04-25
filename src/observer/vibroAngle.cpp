#include "observer.h"

void vibroAngleObserver()
{
    vibroAngle.addObserver([](float value) {
        timber.i("Observer: vibroAngle updated %f", value);
        if (currentMode.get() == WorkMode::VIBRO)
        {
            vibroFr.notifyObservers();
        }
        vibroAngle.save();
    });
}
