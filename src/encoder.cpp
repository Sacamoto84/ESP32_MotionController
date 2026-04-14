#include <EncButton.h>

#define BTN  4
#define ENC0 5
#define ENC1 6

EncButton eb(ENC0, ENC1, BTN);

void encoderInit()
{
    pinMode(BTN, INPUT);
    pinMode(ENC0, INPUT);
    pinMode(ENC1, INPUT);
}

