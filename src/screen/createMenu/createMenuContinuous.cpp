#include "../createMenu.h"

//Создание списка отображения
void createMenuContinuous()
{
    //addMenuElementText(&menuContinuous, "Режим: Постоянный");

    addMenuElementButton(&menuContinuous, "Режим: Постоянный >",[](int){
       timber.i("Нажата кнопка: Перейти на вибро");
       currentScreen = VIBRO;
       currentMode.set(WorkMode::VIBRO);
       update();
   });

    addMenuElementSwitch(&menuContinuous, "Мотор: Вкл", "Мотор: Выкл", &tmcStepperEnable);
    addMenuElementText(&menuContinuous, "Направление: ->");
    addMenuElementEDITINT(&menuContinuous, "Скорость: ", "", &tmcStepperMaxSpeed,0, 100000, 250);
    addMenuElementEDITINT(&menuContinuous, "Амплитуда: ", "", &tmcStepperTarget,0, 1000000, 100);
    ///////////////////////////////////
    addMenuElementButton(&menuContinuous, "Настройка",[](int){
        timber.i("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    });
    //////////////////////////////////
    addMenuElementButton(&menuContinuous, "Перезагрузка",[](int){ esp_restart(); });
    addMenuElementButton(&menuContinuous, "Очистка EEPROM",[](int){
         //eep.erase(4096);
         //eepromDump(0, 4096); 
    });
    //////////////////////////////////
    menuContinuous.ITEMS_COUNT = menuContinuous.items.size();
    menuContinuous.ITEMS_WINDOW = 8;
    menuContinuous.indexEndWindow = menuContinuous.ITEMS_WINDOW - 1;

    //Вариант 1 — самый универсальный
    // menuContinuous.colorBg =          convert24to16(0x16181C);  // RGB565(0x16, 0x18, 0x1C);
    // menuContinuous.colorInactive =    convert24to16(0xAEB6C2);  //RGB565(0xE2,0xE6,0xEA);
    // menuContinuous.colorActive =      convert24to16(0x7FC8FF);  // RGB565(0x7F,0xC8,0xFF);

    // //Вариант 2 — тёплый, мягче для глаз
    // menuContinuous.colorBg =          convert24to16(0x1A1816);  // RGB565(0x16, 0x18, 0x1C);
    // menuContinuous.colorInactive =    convert24to16(0xF0E6D8);  //RGB565(0xE2,0xE6,0xEA);
    // menuContinuous.colorActive =      convert24to16(0xE8B96A);  // RGB565(0x7F,0xC8,0xFF);


    //Вариант 3 — зелёно-серый, очень хорошо прячет плохой чёрный
    menuContinuous.colorBg =          convert24to16(0x151A18);  // RGB565(0x16, 0x18, 0x1C);
    menuContinuous.colorInactive =    convert24to16(0x97AAA0);  //RGB565(0xE2,0xE6,0xEA);
    menuContinuous.colorActive =      convert24to16(0x7FD9B5);  // RGB565(0x7F,0xC8,0xFF);





}
