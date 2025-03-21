#include "createMenu.h"

screenAction menuConfig;
screenAction menuContinuous;
screenAction menuVibro;

//Создание списка отображения
void createMenuContinuous()
{
    addMenuElementText(&menuContinuous, "Режим: Постоянный");
    addMenuElementSwitch(&menuContinuous, "Мотор: Вкл", "Мотор: Выкл", &tmcDriverEnable);
    addMenuElementText(&menuContinuous, "Направление: ->");
    addMenuElementEDITINT(&menuContinuous, "Скорость: ", "", &tmcStepperMaxSpeed,0, 100000, 250);

    itemAction actions;

    actions.type = itemAction::EDITINT_I32;
    actions.text = (char*)"Амплитуда: ";
    actions.testSuffix = (char*)"";
    actions.value = nullptr;
    actions.value = &tmcStepperSetTarget;
    actions.min = 0;
    actions.max = 1000000;
    actions.step = 100;
    menuContinuous.addMenuAction(actions);
    actions.value = nullptr;
    ///////////////////////////////////
    addMenuElementButton(&menuContinuous, "Настройка",[](int){
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    });
    //////////////////////////////////
    addMenuElementButton(&menuContinuous, "Перезагрузка",[](int){ esp_restart(); });
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
    addMenuElementButton(&menuConfig, "<- Назад",[](int){
        Serial2.println("<- Назад"); currentScreen = MAIN; update(); });
    ///////////////////////////////////
    addMenuElementSwitch(&menuConfig, "Мотор: Вкл", "Мотор: Выкл", &tmcDriverEnable);
    ///////////////////////////////////
    addMenuElementSwitch(&menuConfig, "Режим: StealthChop", "Режим: SpreadCycle", &tmcDriverChop);
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
    addMenuElementSwitch(&menuConfig, "Interpolation: Вкл", "Interpolation: Выкл", &tmcDriverInterpolation);
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
    menuVibro.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
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
