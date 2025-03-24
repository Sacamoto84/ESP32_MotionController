#include "tmcGlobal.h"

void tmcInit(){

    pinMode(CS_PIN, INPUT);
    pinMode(SW_MOSI, INPUT);
    pinMode(SW_MISO, INPUT);
    pinMode(SW_SCK, INPUT);

    pinMode(DIAG0, INPUT);
    pinMode(DIAG1, INPUT);

    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    driver.begin();    //  SPI: Init CS pins and possible SW SPI pins
    driver.toff(5); // Enables driver in software
}