#include "createMenu.h"

void addMenuElementText(screenAction* menu, const char* text)
{
    itemAction actions;
    actions.type = itemAction::TEXT;
    actions.text = text;
    menu->addMenuAction(actions);
}

void addMenuElementSwitch(screenAction* menu, const char* textOn, const char* textOff, State<int32_t>* value)
{
    itemAction actions;
    actions.type = itemAction::SWITCH;
    actions.value = value;
    actions.textOn = textOn;
    actions.textOff = textOff;
    menu->addMenuAction(actions);
}

void addMenuElementEDITINT(screenAction* menu, const char* text, const char* textSuffix, State<int32_t>* value,
                           int32_t min, int32_t max, int32_t step)
{
    itemAction actions;
    actions.type = itemAction::EDITINT;
    actions.text = text;
    actions.testSuffix = textSuffix;
    actions.value = value;
    actions.min = min;
    actions.max = max;
    actions.step = step;
    menu->addMenuAction(actions);
}

void addMenuElementButton(screenAction* menu, const char* text, CallbackType callback)
{
    itemAction actions;
    actions.type = itemAction::BUTTON;
    actions.text = text;
    actions.callback = callback;
    menu->addMenuAction(actions);
    actions.callback = nullptr;
}

