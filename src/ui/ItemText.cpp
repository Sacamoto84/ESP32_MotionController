#include "ui.h"

void ItemText(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y){
    String str = String(item->text);
    if (line == index) {
        Text(str, x, y, item->colorActive, item->colorBg);
    } else
        Text(str, x, y, item->colorInactive, item->colorBg);

  }
