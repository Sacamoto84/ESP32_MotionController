#include "ui.h"

void Text(const String& text, const int16_t x, const int16_t y, uint16_t colorText, uint16_t colorBg, bool bgfill) {
    tft.setTextColor(colorText, colorBg, bgfill);
    tft.setCursor(x, y);
    tft.println(text);
}
