#ifndef INA226_MONITOR_H
#define INA226_MONITOR_H

#include <Arduino.h>

struct Ina226Reading
{
    bool ready = false;
    bool overflow = false;
    uint8_t i2cError = 0;
    float voltageV = 0.0f;
    float currentA = 0.0f;
    uint32_t updatedMs = 0;
};

void initIna226Monitor();
void tickIna226Monitor();
Ina226Reading getIna226Reading();
String ina226OverlayText();

#endif
