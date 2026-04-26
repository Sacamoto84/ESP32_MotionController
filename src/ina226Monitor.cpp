#include "ina226Monitor.h"

#include "global.h"

#include <INA226_WE.h>
#include <Wire.h>

namespace
{
constexpr uint8_t kIna226Address = 0x40;
constexpr uint8_t kIna226SdaPin = 48;
constexpr uint8_t kIna226SclPin = 47;
constexpr float kIna226ShuntOhms = 0.01f;
constexpr uint32_t kIna226PollIntervalMs = 500;
constexpr uint32_t kIna226RetryIntervalMs = 2000;

INA226_WE ina226(&Wire, kIna226Address);
Ina226Reading reading;
portMUX_TYPE readingMux = portMUX_INITIALIZER_UNLOCKED;
bool initialized = false;
uint32_t lastPollMs = 0;

String formatFloat(float value, uint8_t decimals)
{
    char buffer[16];
    dtostrf(value, 0, decimals, buffer);
    return String(buffer);
}

void setReading(const Ina226Reading& next)
{
    portENTER_CRITICAL(&readingMux);
    reading = next;
    portEXIT_CRITICAL(&readingMux);
}

bool beginIna226()
{
    Wire.begin(kIna226SdaPin, kIna226SclPin);
    Wire.setClock(400000);

    if (!ina226.init())
    {
        timber.e("INA226 init failed on SDA=%d SCL=%d addr=0x%02X", kIna226SdaPin, kIna226SclPin, kIna226Address);
        initialized = false;
        Ina226Reading failed;
        failed.ready = false;
        failed.i2cError = ina226.getI2cErrorCode();
        failed.updatedMs = millis();
        setReading(failed);
        return false;
    }

    ina226.setResistorRange(kIna226ShuntOhms);
    ina226.setAverage(INA226_AVERAGE_16);
    ina226.setConversionTime(INA226_CONV_TIME_1100);
    ina226.setMeasureMode(INA226_CONTINUOUS);
    ina226.waitUntilConversionCompleted();

    initialized = true;
    timber.i("INA226 ready: SDA=%d SCL=%d shunt=%.3f Ohm", kIna226SdaPin, kIna226SclPin, kIna226ShuntOhms);
    return true;
}

void readIna226()
{
    Ina226Reading next;
    next.ready = true;
    next.voltageV = ina226.getBusVoltage_V();
    next.currentA = ina226.getCurrent_A();
    next.overflow = ina226.overflow;
    next.i2cError = ina226.getI2cErrorCode();
    next.updatedMs = millis();

    if (next.i2cError != 0)
    {
        next.ready = false;
        initialized = false;
        timber.e("INA226 I2C error: %d", next.i2cError);
    }

    setReading(next);
}
}

void initIna226Monitor()
{
    beginIna226();
    lastPollMs = millis();
}

void tickIna226Monitor()
{
    const uint32_t now = millis();
    const uint32_t interval = initialized ? kIna226PollIntervalMs : kIna226RetryIntervalMs;
    if (now - lastPollMs < interval)
    {
        return;
    }
    lastPollMs = now;

    if (!initialized)
    {
        beginIna226();
        update();
        return;
    }

    readIna226();
    update();
}

Ina226Reading getIna226Reading()
{
    Ina226Reading snapshot;
    portENTER_CRITICAL(&readingMux);
    snapshot = reading;
    portEXIT_CRITICAL(&readingMux);
    return snapshot;
}

String ina226OverlayText()
{
    const Ina226Reading snapshot = getIna226Reading();
    if (!snapshot.ready)
    {
        return "INA --";
    }

    String text = formatFloat(snapshot.voltageV, 2);
    text += "V ";
    text += formatFloat(snapshot.currentA, 2);
    text += "A";

    if (snapshot.overflow)
    {
        text += "!";
    }

    return text;
}
