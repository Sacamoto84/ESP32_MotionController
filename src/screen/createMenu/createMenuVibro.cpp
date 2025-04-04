#include "../createMenu.h"

void createMenuVibro()
{

    addMenuElementButton(&menuVibro, "Режим: Вибро >",[](int){
       timber.i("Нажата кнопка: Перейти на Постоянные");
       currentScreen = MAIN;
       currentMode = WorkMode::CONTINUOUS;
       update();
   });

    addMenuElementSwitch(&menuVibro, "Мотор: Вкл", "Мотор: Выкл", &tmcStepperEnable);

    addMenuElementEditFloat(&menuVibro, "Частота: ", "", &vibroFr,1.0f, 200.0f, 1.0f);
    addMenuElementEditFloat(&menuVibro, "Угол: ", "", &vibroAngle,1.0f, 359.0f, 1.0f);

    itemAction actions;
    actions.type = itemAction::EDITINTMICROSTEP;
    actions.value = &tmcDriverMicrostep;
    actions.text = "Микрошаг: 1/";
    actions.testSuffix = (char*)"";
    actions.min = 1;
    actions.max = 256;
    actions.step = 1;
    menuVibro.addMenuAction(actions);

    ///////////////////////////////////
    addMenuElementButton(&menuVibro, "Настройка",[](int){
       Serial2.println("Нажата кнопка: Настройка");
       currentScreen = CONFIG;
       update();
    });
    //////////////////////////////////
    addMenuElementButton(&menuVibro, "Перезагрузка",[](int){ esp_restart(); });
    //////////////////////////////////
    menuVibro.ITEMS_COUNT = menuVibro.items.size();
    menuVibro.ITEMS_WINDOW = 6;
    menuVibro.indexEndWindow = menuVibro.ITEMS_WINDOW - 1;
}