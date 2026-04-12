#include "ui.h"

void ItemMICROSTEP(int line, int index, itemAction* item, bool* isSelect, int16_t x, int16_t y)
{
    auto a = *isSelect;

    if (line == index)
    {
        if (eb.press()){ a = !a; *isSelect = a; }

        if (a)
        {
            if (eb.right()){
                uint16_t t = item->value->get() * 2;
                if (t >= item->max) t = item->max;
                item->value->set(t);
            }

            if (eb.left()){
                uint16_t t = item->value->get() / 2;
                if (t <= item->min) t = item->min;
                item->value->set(t);
            }
        }
    }

    String str = String(item->text) + item->value->get() + item->testSuffix;

    if (line == index)
    {
        if (!a)
            Text(str, x, y, item->colorActive, item->colorBg);
        else
            Text(str, x, y, item->colorBg, item->colorActive, true);
    }
    else
        Text(str, x, y, item->colorInactive, item->colorBg);


}
