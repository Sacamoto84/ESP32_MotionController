#include "ui.h"

void ITEM(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y) {

  	//////////////////////////////////////////////
    if (item->type == itemAction::SWITCH) {
        ItemSwitch(line, index, item, isSelect, x, y);
        return;
    }
    //////////////////////////////////////////////
    if ((item->type == itemAction::EDITINT) || (item->type == itemAction::EDITINTMICROSTEP)) {
        auto a = *isSelect;
        if (line == index) {
            if (eb.press()) {
                a = !a;
                *isSelect = a;
            }
            if (a) {
                if (item->type == itemAction::EDITINT) {
                    if (eb.right()) {
                        uint16_t t = item->value->get() + item->step;
                        if (t >= item->max)
                            t = item->max;
                        item->value->set(t);
                    }
                    if (eb.left()) {
                        uint16_t t = item->value->get() - item->step;
                        if (t <= item->min)
                            t = item->min;
                        item->value->set(t);
                    }
                }

                if (item->type == itemAction::EDITINTMICROSTEP) {
                    if (eb.right()) {
                        uint16_t t = item->value->get() * 2;
                        if (t >= item->max)
                            t = item->max;
                        item->value->set(t);
                    }
                    if (eb.left()) {
                        uint16_t t = item->value->get() / 2;
                        if (t <= item->min)
                            t = item->min;
                        item->value->set(t);
                    }
                }
            }
        }
        String str11 = String(item->text) + item->value->get() + item->testSuffix;
        if (line == index) {
            if (!a)
                Text(str11, x, y, item->colorActive, item->colorBg);
            else
                Text(str11, x, y, item->colorBg, item->colorActive, true);
        } else
            Text(str11, x, y, item->colorInactive, item->colorBg);
        return;
    }



	//////////////////////////////////////////////
    if (item->type == itemAction::EDITINT_I32) {
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

        return;
    }
	//////////////////////////////////////////////
    if (item->type == itemAction::TEXT) {
      ItemText(line, index, item, isSelect, x, y);
      return;
    }
	//////////////////////////////////////////////
    if (item->type == itemAction::BUTTON) {
		ItemButton(line, index, item, isSelect, x, y);
        return;
    }
}