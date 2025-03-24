#include "../createMenu.h"

//Создание списка настройки
void createMenuConfig()
{
    itemAction actions;

    addMenuElementButton(&menuConfig, "<- Назад",[](int){
        Serial2.println("<- Назад"); currentScreen = MAIN; update(); });
    addMenuElementSwitch(&menuConfig, "Мотор: Вкл", "Мотор: Выкл", &tmcStepperEnable);
    addMenuElementSwitch(&menuConfig, "Режим: StealthChop", "Режим: SpreadCycle", &tmcDriverChop);
    addMenuElementEDITINT(&menuConfig, "Ток: ", " мА", &tmcDriverCurrent,100, 3100, 100);
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
    addMenuElementSwitch(&menuConfig, "Interpolation: Вкл", "Interpolation: Выкл", &tmcInterpolation);
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