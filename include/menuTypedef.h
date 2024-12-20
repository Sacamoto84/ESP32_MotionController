#ifndef ESP32_MOTIONCONTROL_MENUTYPEDEF_H
#define ESP32_MOTIONCONTROL_MENUTYPEDEF_H

#include <Arduino.h>
#include <string>
#include <vector> // Для std::vector
#include <functional>
#include "state.h"
#include "TFT_color.h"

// Определяем тип коллбека
typedef void (*CallbackType)(int);

struct itemAction {

    enum ActionType {
        NONE,      // Элемент не используется
        SWITCH,    // Действие SWITCH
        EDITINT,    // Действие EDITINT
        EDITINT_I32,    // Действие EDITINT
        EDITINTMICROSTEP,
        TEXT,
        BUTTON
    } type = NONE; // По умолчанию элемент пустой

    State<uint16_t> *value = nullptr;
    State<int32_t> *value_i32 = nullptr;

    char * textOn;
    char * textOff;
    char * testSuffix;

    String text;
    //String correction;

    int32_t min=0;
    int32_t max=1;
    int16_t step=1;

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
    uint16_t colorBg = RGB565(0, 0, 128);

    std::vector<itemAction> items; // Динамический список элементов MenuAction

    // Метод для добавления нового действия
    void addMenuAction(const itemAction& action) {
        items.push_back(action);
    }



};


#endif //ESP32_MOTIONCONTROL_MENUTYPEDEF_H
