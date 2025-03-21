//
// Created by Ivan on 20.03.2025.
//

#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <EncButton.h>
#include <Arduino.h>
#include <string>
#include <vector> // Для std::vector
#include <functional>
#include "state.h"
#include "TFT_color.h"

// Определяем тип коллбека
typedef void (*CallbackType)(int);

struct itemAction
{
    enum ActionType
    {
        NONE, // Элемент не используется
        SWITCH, // Действие SWITCH
        EDITINT, // Действие EDITINT
        EDITINT_I32, // Действие EDITINT
        EDITINTMICROSTEP,
        TEXT,
        BUTTON
    } type = NONE; // По умолчанию элемент пустой

    State<int32_t>* value = nullptr;

    String textOn;
    String textOff;
    String testSuffix;

    String text;
    //String correction;

    int32_t min = 0;
    int32_t max = 1;
    int32_t step = 1;

    //Цвета
    uint16_t colorActive;
    uint16_t colorInactive;
    uint16_t colorBg;

    bool skipping = false;

    CallbackType callback = nullptr; // Коллбек как указатель на функцию

    void executeCallback(const int data) const
    {
        if (callback)
        {
            callback(data);
        }
    }
};


extern EncButton eb;
extern TFT_eSPI tft;

extern void ITEM(int line, int index, itemAction* item, bool* isSelect, int16_t x, int16_t y);
extern void Text(const String& text, const int16_t x = 0, const int16_t y = 0, uint16_t colorText = 0,
                 uint16_t colorBg = 0xFFFF, bool bgfill = false);
extern void ItemSwitch(int line, int index, itemAction* item, bool* isSelect, int16_t x, int16_t y);
extern void ItemText(int line, int index, itemAction* item, bool* isSelect, int16_t x, int16_t y);
extern void ItemButton(int line, int index, itemAction* item, bool* isSelect, int16_t x, int16_t y);

#endif //UI_H
