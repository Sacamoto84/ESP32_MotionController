#include <Arduino.h>
#include "ui.h"

void ItemButton(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y){

    if (line == index)
        Text(item->text, x, y, item->colorActive, item->colorBg);
    else
        Text(item->text, x, y, item->colorInactive, item->colorBg);

    if ((line == index) && eb.press()) {
        eb.clear();
        item->executeCallback(0);
    }

  }
