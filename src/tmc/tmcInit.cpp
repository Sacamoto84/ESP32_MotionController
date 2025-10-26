#include "tmcGlobal.h"
#include "global.h"

void tmcInit(){
    timber.i("Инициализация TMC...");

    pinMode(CS_PIN, OUTPUT);
    pinMode(SW_MOSI, OUTPUT);
    pinMode(SW_MISO, INPUT);
    pinMode(SW_SCK, OUTPUT);

    pinMode(DIAG0, INPUT);
    pinMode(DIAG1, INPUT);

    pinMode(EN_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);

    driver.begin();    //  SPI: Init CS pins and possible SW SPI pins
    driver.toff(5); // Enables driver in software

    timber.i("TMC инициализирован");
}