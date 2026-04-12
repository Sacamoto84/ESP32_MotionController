#include "ui.h"

void ItemSwitch(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y){
        if ((line == index) && (eb.press())) {
            uint16_t a = item->value->get();
            a = !a;
            item->value->set(a);
        }
        auto s0 = (item->value->get()) ? &item->textOn : &item->textOff;
        if (line == index)
            Text(*s0, x, y, item->colorActive, item->colorBg);
        else
            Text(*s0, x, y, item->colorInactive, item->colorBg);
  }
