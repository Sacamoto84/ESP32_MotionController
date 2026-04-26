#include "observer.h"

#include <math.h>

#include "motionConfig.h"

namespace
{
constexpr float kRpsStepPrecision = 10.0f;

float getContinuousMaxRps(float microstep)
{
    if (microstep <= 0.0f)
    {
        return 0.0f;
    }

    const float maxRps = kStepperMaxStepRateHz / (kMotorFullStepsPerRevolution * microstep);
    return floorf(maxRps * kRpsStepPrecision) / kRpsStepPrecision;
}

void applyContinuousSpeed()
{
    if (currentMode.get() != WorkMode::CONTINUOUS)
    {
        return;
    }

    const float microstep = static_cast<float>(tmcDriverMicrostep.get());
    const float maxRps = getContinuousMaxRps(microstep);
    float rps = constRps.get();
    if (rps < 0.0f)
    {
        rps = -rps;
    }

    if (maxRps > 0.0f && rps > maxRps)
    {
        timber.w("Continuous speed limited: requested %.2f rps, max %.2f rps at microstep 1/%d",
                 rps, maxRps, tmcDriverMicrostep.get());
        constRps.set(maxRps);
        return;
    }

    const int32_t direction = constDirection.get() ? 1 : -1;

    portENTER_CRITICAL(&stepperMux);
    if (rps <= 0.0f || microstep <= 0.0f)
    {
        stepper.setSpeed(static_cast<int32_t>(0));
    }
    else
    {
        stepper.setSpeed(kMotorFullStepsPerRevolution * microstep * rps * direction);
    }
    portEXIT_CRITICAL(&stepperMux);
}
} // namespace

void constDirectionObserver()
{
    constDirection.addObserver([](int32_t value) {
        timber.i("Observer: constDirection updated %d", value);
        applyContinuousSpeed();
    });
}

void constRpsObserver()
{
    constRps.addObserver([](float value) {
        timber.i("Observer: constRps updated %f", value);
        applyContinuousSpeed();
    });
}
