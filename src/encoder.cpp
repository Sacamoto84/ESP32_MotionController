#include <EncButton.h>

#include "driver/pulse_cnt.h"

#define BTN   4
#define ENC_A 5
#define ENC_B 6

EncButton eb(ENC_A, ENC_B, BTN);

void encoderInit()
{
    pinMode(BTN, INPUT);
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);
}
