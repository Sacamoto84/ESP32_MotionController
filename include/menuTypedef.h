#ifndef ESP32_MOTIONCONTROL_MENUTYPEDEF_H
#define ESP32_MOTIONCONTROL_MENUTYPEDEF_H

#include <Arduino.h>
#include <string>
#include <vector> // Для std::vector
#include <functional>
#include "state.h"
#include "TFT_color.h"

//// Типы функций
//using SwitchFunction = void(*)(int index, int line, bool *isSelect, State<uint16_t> *value, String text_on, String text_off, int x, int y,
//                               uint16_t colorActive, uint16_t colorInactive, uint16_t colorBg);
//
//using EditIntFunction = void(*)(int index, int line, bool *isSelect, State<uint16_t> *value, uint16_t min, uint16_t max, uint16_t step,
//                                String text, int x, int y, uint16_t colorActive, uint16_t colorInactive, uint16_t colorBg, const String& correction);

// Определяем тип коллбека
typedef void (*CallbackType)(int);

struct itemAction {

    enum ActionType {
        NONE,      // Элемент не используется
        SWITCH,    // Действие SWITCH
        EDITINT,    // Действие EDITINT
        EDITINTMICROSTEP,
        TEXT,
        BUTTON
    } type = NONE; // По умолчанию элемент пустой

    State<uint16_t> *value;

    String textOn="";
    String textOff="";
    String text="";
    //String correction;

    uint16_t min=0;
    uint16_t max=1;
    uint16_t step=1;

     //Цвета
    uint16_t colorActive;
    uint16_t colorInactive ;
    uint16_t colorBg;

    bool skipping = false;

    CallbackType callback = nullptr; // Коллбек как указатель на функцию

    void executeCallback(int data) const {
        if (callback) {
            callback(data);
        }
    }
};



struct screenAction {

    bool isSelect = false;
    int line = 0;

    int ITEMS_COUNT = 2;  // Количество елементов в списке
    int ITEMS_WINDOW = 2; // Количество отображаемых елементов

    int height = 26;
    int startY = 5;
    int startX = 6;

    int indexStartWindow = 0;
    int indexEndWindow = ITEMS_WINDOW - 1;

    //Цвета
    uint16_t colorActive= Yellow;
    uint16_t colorInactive = DarkGrey;
    uint16_t colorBg = RGB565(0, 0, 128);

    std::vector<itemAction> items; // Динамический список элементов MenuAction

    // Метод для добавления нового действия
    void addMenuAction(const itemAction& action) {
        items.push_back(action);
    }



};


#endif //ESP32_MOTIONCONTROL_MENUTYPEDEF_H
