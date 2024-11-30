//
// Created by user on 30.11.2024.
//

#include "createMenu.h"

//
screenAction menuConfig;
screenAction menuMain;

//Создание списка отображения
void createMenuMain() {
    itemAction actions;

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Режим: Постоянный";
    menuMain.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    menuMain.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Направление: ->";
    menuMain.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Скорость: 20 имп/c";
    menuMain.addMenuAction(actions);

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Настройка";
    actions.callback = [](int data) {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuMain.addMenuAction(actions);
    actions.callback = nullptr;
    //////////////////////////////////

    //////////////////////////////////
    menuMain.ITEMS_COUNT = menuMain.items.size();
    menuMain.ITEMS_WINDOW = 6;
    menuMain.indexEndWindow = menuMain.ITEMS_WINDOW - 1;
}
//Создание списка настройки
void createMenuConfig() {

    itemAction actions;

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "<- Назад";
    actions.callback = [](int data) {
        Serial2.println("Нажата кнопка: Назад");
        currentScreen = MAIN;
        update();
    };
    menuConfig.addMenuAction(actions);
    actions.callback = nullptr;
    ///////////////////////////////////
    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    actions.skipping = false;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverChop;
    actions.textOn = (char*)"Режим: StealthChop";
    actions.textOff = (char*)"Режим: SpreadCycle";
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::EDITINT;
    actions.value = &tmcDriverCurrent;
    actions.text = "Ток: ";
    actions.testSuffix = (char *)" мА";
    actions.min = 100;
    actions.max = 3100;
    actions.step = 100;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::EDITINTMICROSTEP;
    actions.value = &tmcDriverMicrostep;
    actions.text = "Микрошаг: 1/";
    actions.testSuffix = (char *)"";
    actions.min = 1;
    actions.max = 256;
    actions.step = 1;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverInterpolation;
    actions.textOn = (char*)"Interpolation: Вкл";
    actions.textOff = (char*)"Interpolation: Выкл";
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.text = "Текст0";
    actions.skipping = true;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.text = "Текст1";
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Button";
    // Устанавливаем лямбду как коллбек
    actions.callback = [](int data) {
        Serial2.println("Нажата кнопка");
    };
    menuConfig.addMenuAction(actions);
    actions.callback = nullptr;
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.text = "Текст2";
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    menuConfig.ITEMS_COUNT = menuConfig.items.size();
    menuConfig.ITEMS_WINDOW = 6;
    menuConfig.indexEndWindow = menuConfig.ITEMS_WINDOW - 1;

}