#include "observer.h"

namespace
{
uint16_t normalizeMicrostep(int32_t value)
{
    if (value <= 1)
    {
        return 1;
    }

    if (value > 256)
    {
        return 256;
    }

    uint16_t normalized = 1;
    while (normalized < static_cast<uint16_t>(value))
    {
        normalized <<= 1;
    }

    return normalized;
}

void applyDriverMicrostep(uint16_t microstep)
{
    if (microstep == 1)
    {
        // TMC2160 full-step mode is MRES=8. Set it directly instead of
        // relying on the library's "microsteps(0)" convention.
        driver.mres(8);
        return;
    }

    driver.microsteps(microstep);
}

void resetVibroPlanner()
{
    portENTER_CRITICAL(&stepperMux);
    stepper.brake();
    stepper.setCurrent(0);
    portEXIT_CRITICAL(&stepperMux);

    vibroDir.store(false);
}
} // namespace

void tmcDriverMicrostepObserver()
{
    tmcDriverMicrostep.addObserver([](int32_t value) {
        const uint16_t microstep = normalizeMicrostep(value);
        if (value != static_cast<int32_t>(microstep))
        {
            tmcDriverMicrostep.set(static_cast<int32_t>(microstep));
            return;
        }

        if (currentMode.get() == WorkMode::VIBRO)
        {
            resetVibroPlanner();
        }

        applyDriverMicrostep(microstep);
        timber.i("Observer: tmcDriverMicrostep requested 1/%u, MRES=%u",
                 microstep, driver.mres());
        tmcDriverMicrostep.save();

        if (currentMode.get() == WorkMode::VIBRO)
        {
            vibroAngle.notifyObservers();
            vibroFr.notifyObservers();
        }
        else if (currentMode.get() == WorkMode::CONTINUOUS)
        {
            constRps.notifyObservers();
        }
    });
}
