
#include "ui.h"

void ItemEditFloat(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y){

    auto a = *isSelect;
    if (line == index) {
        if (eb.press()) { a = !a; *isSelect = a; }
        if (a) {
            if (eb.right()) {
                float t = item->valueF->get() + item->stepF;
                if (t >= item->maxF) t = item->maxF; item->valueF->set(t);
            }
            if (eb.left()) {
                float t = item->valueF->get() - item->stepF;
                if (t <= item->minF) t = item->minF; item->valueF->set(t);
            }
        }
    }
    String str11 = String(item->text) + item->valueF->get() + item->testSuffix;
    if (line == index) {
        if (!a) Text(str11, x, y, item->colorActive, item->colorBg);
        else    Text(str11, x, y, item->colorBg, item->colorActive, true);
    } else      Text(str11, x, y, item->colorInactive, item->colorBg);

}