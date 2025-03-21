#include "ui.h"

void ItemEDITINT(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y){

    auto a = *isSelect;
    if (line == index) {
        if (eb.press()) { a = !a; *isSelect = a; }
        if (a) {
            if (eb.right()) {
                int32_t t = item->value->get() + item->step;
                if (t >= item->max) t = item->max; item->value->set(t);
            }
            if (eb.left()) {
                int32_t t = item->value->get() - item->step;
                if (t <= item->min) t = item->min; item->value->set(t);
            }
        }
    }
    String str11 = String(item->text) + item->value->get() + item->testSuffix;
    if (line == index) {
        if (!a) Text(str11, x, y, item->colorActive, item->colorBg);
        else    Text(str11, x, y, item->colorBg, item->colorActive, true);
    } else      Text(str11, x, y, item->colorInactive, item->colorBg);

  }