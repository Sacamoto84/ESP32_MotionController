//
// Created by user on 30.11.2024.
//

#ifndef ESP32_MOTIONCONTROL_CREATEMENU_H
#define ESP32_MOTIONCONTROL_CREATEMENU_H

#include "global.h"
#include "menuTypedef.h"

enum CurrenScreen {
    MAIN,
    CONFIG,
    VIBRO,
};

extern screenAction menuConfig;
extern screenAction menuContinuous;
extern screenAction menuVibro;

extern enum CurrenScreen currentScreen;

void createMenuConfig();
void createMenuContinuous();
void createMenuVibro();



#endif //ESP32_MOTIONCONTROL_CREATEMENU_H
