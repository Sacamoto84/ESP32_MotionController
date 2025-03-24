#ifndef ESP32_MOTIONCONTROL_MENUTYPEDEF_H
#define ESP32_MOTIONCONTROL_MENUTYPEDEF_H

#include <Arduino.h>
#include <vector> // Для std::vector
#include "TFT_color.h"
#include "../ui/ui.h"

struct screenAction {

    bool isSelect = false;
    int line = 0;

    uint ITEMS_COUNT = 2;  // Количество елементов в списке
    int ITEMS_WINDOW = 2; // Количество отображаемых елементов

    int16_t height = 21; //26
    int16_t startY = 5;
    int16_t startX = 6;

    int indexStartWindow = 0;
    int indexEndWindow = ITEMS_WINDOW - 1;

    //Цвета
    uint16_t colorActive= Yellow;
    uint16_t colorInactive = DarkGrey;
    uint16_t colorBg = RGB565(0, 0, 0);

    std::vector<itemAction> items; // Динамический список элементов MenuAction

    // Метод для добавления нового действия
    void addMenuAction(const itemAction& action) {
        items.push_back(action);
    }
};


#endif //ESP32_MOTIONCONTROL_MENUTYPEDEF_H
