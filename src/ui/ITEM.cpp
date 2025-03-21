#include "ui.h"

void ITEM(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y) {
    if (item->type == itemAction::SWITCH){ ItemSwitch(line, index, item, isSelect, x, y); return; }
    if (item->type == itemAction::EDITINTMICROSTEP) { ItemMICROSTEP(line, index,  item, isSelect, x, y); return; }
    if (item->type == itemAction::EDITINT) { ItemEDITINT(line, index, item, isSelect, x, y); return; }
    if (item->type == itemAction::TEXT) { ItemText(line, index, item, isSelect, x, y); return; }
    if (item->type == itemAction::BUTTON) { ItemButton(line, index, item, isSelect, x, y); return; }
}