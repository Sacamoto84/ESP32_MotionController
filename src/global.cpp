#include "global.h"

State<uint16_t> tmcDriverEnable(0);
State<uint16_t> tmcDriverChop(0);
State<uint16_t> tmcDriverCurrent(1000); //Ток двайвера
State<uint16_t> tmcDriverMicrostep(16); //Микрошаг
State<uint16_t> tmcDriverInterpolation(0);

#define EN_PIN 14 // Enable

void observer() {
    tmcDriverEnable.addObserver([](uint16_t value) {
        Serial2.printf("Observer: tmcDriverEnable изменен %d\n", value);

        if (value)
            digitalWrite(EN_PIN, LOW);
        else
            digitalWrite(EN_PIN, HIGH);

    });

    tmcDriverChop.addObserver([](uint16_t value) {
        Serial2.println("Observer: tmcDriverChop updated");

    });

    tmcDriverCurrent.addObserver([](uint16_t value) {
        Serial2.printf("Observer: tmcDriverCurrent updated %d\n", value);
        driver.rms_current(value); // Set motor RMS current
    });

}



