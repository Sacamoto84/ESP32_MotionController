#include "../createMenu.h"

//Создание списка отображения
void createMenuContinuous()
{
    addMenuElementText(&menuContinuous, "Режим: Постоянный");
    addMenuElementSwitch(&menuContinuous, "Мотор: Вкл", "Мотор: Выкл", &tmcStepperEnable);
    addMenuElementText(&menuContinuous, "Направление: ->");
    addMenuElementEDITINT(&menuContinuous, "Скорость: ", "", &tmcStepperMaxSpeed,0, 100000, 250);
    addMenuElementEDITINT(&menuContinuous, "Амплитуда: ", "", &tmcStepperTarget,0, 1000000, 100);
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
