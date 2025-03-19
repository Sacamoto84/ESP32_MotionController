//
// Created by user on 30.11.2024.
//

#include "createMenu.h"

//
screenAction menuConfig;
screenAction menuContinuous;
screenAction menuVibro;

//Создание списка отображения
void createMenuContinuous()
{
    itemAction actions;

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Режим: Постоянный";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Направление: ->";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::EDITINT;
    actions.text = (char*)"Скорость: ";
    actions.testSuffix = (char*)"";
    actions.value = &tmcStepperMaxSpeed;
    actions.min = 0;
    actions.max = 100000;
    actions.step = 250;
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::EDITINT_I32;
    actions.text = (char*)"Амплитуда: ";
    actions.testSuffix = (char*)"";
    actions.value = nullptr;
    actions.value_i32 = &tmcStepperSetTarget;
    actions.min = 0;
    actions.max = 1000000;
    actions.step = 100;
    menuContinuous.addMenuAction(actions);
    actions.value_i32 = nullptr;
    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Настройка";
    actions.callback = [](int data)
    {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuContinuous.addMenuAction(actions);
    actions.callback = nullptr;
    //////////////////////////////////

    actions.type = itemAction::BUTTON;
    actions.text = "Перезагрузка";
    actions.callback = [](int)
    {
        esp_restart();
    };
    menuContinuous.addMenuAction(actions);
    actions.callback = nullptr;

    //////////////////////////////////
    menuContinuous.ITEMS_COUNT = menuContinuous.items.size();
    menuContinuous.ITEMS_WINDOW = 6;
    menuContinuous.indexEndWindow = menuContinuous.ITEMS_WINDOW - 1;
}

//Создание списка настройки
void createMenuConfig()
{
    itemAction actions;

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "<- Назад";
    actions.callback = [](int data)
    {
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
    actions.testSuffix = (char*)" мА";
    actions.min = 100;
    actions.max = 3100;
    actions.step = 100;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::EDITINTMICROSTEP;
    actions.value = &tmcDriverMicrostep;
    actions.text = "Микрошаг: 1/";
    actions.testSuffix = (char*)"";
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
    actions.callback = [](int data)
    {
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

void createMenuVibro()
{
    itemAction actions;

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Режим: Вибро";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = (char*)"Мотор: Вкл";
    actions.textOff = (char*)"Мотор: Выкл";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Направление: ->";
    menuContinuous.addMenuAction(actions);

    actions.type = itemAction::TEXT;
    actions.text = (char*)"Скорость: 20 имп/c";
    menuContinuous.addMenuAction(actions);

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Настройка";
    actions.callback = [](int data)
    {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuContinuous.addMenuAction(actions);
    actions.callback = nullptr;
    //////////////////////////////////

    actions.type = itemAction::BUTTON;
    actions.text = "Перезагрузка";
    actions.callback = [](int data)
    {
        esp_restart();
    };
    menuContinuous.addMenuAction(actions);
    actions.callback = nullptr;

    //////////////////////////////////
    menuContinuous.ITEMS_COUNT = menuContinuous.items.size();
    menuContinuous.ITEMS_WINDOW = 6;
    menuContinuous.indexEndWindow = menuContinuous.ITEMS_WINDOW - 1;
}
