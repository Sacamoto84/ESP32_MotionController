#include "../createMenu.h"

void createMenuVibro()
{
    itemAction actions;

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Режим: Вибро";
    menuVibro.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcStepperEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    menuVibro.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Направление: ->";
    menuVibro.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Скорость: 20 имп/c";
    menuVibro.addMenuAction(actions);

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Настройка";
    actions.callback = [](int data)
    {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuVibro.addMenuAction(actions);
    actions.callback = nullptr;
    //////////////////////////////////

    actions.type = itemAction::BUTTON;
    actions.text = "Перезагрузка";
    actions.callback = [](int data)
    {
        esp_restart();
    };
    menuVibro.addMenuAction(actions);
    actions.callback = nullptr;

    //////////////////////////////////
    menuVibro.ITEMS_COUNT = menuVibro.items.size();
    menuVibro.ITEMS_WINDOW = 6;
    menuVibro.indexEndWindow = menuVibro.ITEMS_WINDOW - 1;
}