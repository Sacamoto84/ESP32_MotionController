#include "ui.h"

void Text(const String& text, const int16_t x, const int16_t y, uint16_t colorText, uint16_t colorBg, bool bgfill) {
    spr.setTextColor(colorText, colorBg, bgfill);
    spr.setCursor(x, y);
    spr.println(text);
    
}
