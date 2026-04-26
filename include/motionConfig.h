#ifndef MOTION_CONFIG_H
#define MOTION_CONFIG_H

#include <Arduino.h>

constexpr uint32_t kStepperTimerFrequencyHz = 1000000;
constexpr uint32_t kStepperIdlePeriodUs = 1000;
constexpr uint32_t kStepperMinPeriodUs = 8;
constexpr float kMotorFullStepsPerRevolution = 200.0f;
constexpr float kMotorStepAngleDegrees = 360.0f / kMotorFullStepsPerRevolution;
constexpr float kStepperMaxStepRateHz = 1000000.0f / static_cast<float>(kStepperMinPeriodUs);

#endif
