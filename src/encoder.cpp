#include <EncButton.h>

#define BTN 19
#define ENC0 22
#define ENC1 21
EncButton eb(ENC0, ENC1, BTN);

void encoderInit()
{
    pinMode(BTN, INPUT);
    pinMode(ENC0, INPUT);
    pinMode(ENC1, INPUT);
}

