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
};

extern screenAction menuConfig;
extern screenAction menuMain;

extern enum CurrenScreen currentScreen;

void createMenuConfig();
void createMenuMain();



#endif //ESP32_MOTIONCONTROL_CREATEMENU_H
