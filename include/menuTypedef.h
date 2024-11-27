#ifndef ESP32_MOTIONCONTROL_MENUTYPEDEF_H
#define ESP32_MOTIONCONTROL_MENUTYPEDEF_H

#include <Arduino.h>
#include <string>
#include <vector> // Для std::vector
#include <functional>
#include "state.h"

// Типы функций
using SwitchFunction = void(*)(int index, int line, bool *isSelect, State<uint16_t> *value, String text_on, String text_off, int x, int y,
                               uint16_t colorActive, uint16_t colorInactive, uint16_t colorBg);

using EditIntFunction = void(*)(int index, int line, bool *isSelect, State<uint16_t> *value, uint16_t min, uint16_t max, uint16_t step,
                                String text, int x, int y, uint16_t colorActive, uint16_t colorInactive, uint16_t colorBg, const String& correction);


struct MenuAction{

    enum ActionType {
        NONE,      // Элемент не используется
        SWITCH,    // Действие SWITCH
        EDITINT    // Действие EDITINT
    } type = NONE; // По умолчанию элемент пустой

    union {
        SwitchFunction switchFunc;   // Указатель на функцию SWITCH
        EditIntFunction editIntFunc; // Указатель на функцию EDITINT
    };

    // Дополнительные параметры для вызова функций
    int index;
    int line;
    bool *isSelect;
    State<uint16_t> *value;
    uint16_t min, max, step;
    String textOn, textOff, text, correction;
    int x, y;
    uint16_t colorActive, colorInactive, colorBg;

};

struct screenAction {


    std::vector<MenuAction> menu; // Динамический список элементов MenuAction

    // Метод для добавления нового действия
    void addMenuAction(const MenuAction& action) {
        menu.push_back(action);
    }


};




#endif //ESP32_MOTIONCONTROL_MENUTYPEDEF_H
