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

void addMenuElementText(screenAction* menu, const char* text, bool skipping = false);
void addMenuElementSwitch(screenAction* menu, const char* textOn, const char* textOff, State<int32_t>* value);
void addMenuElementEDITINT(screenAction* menu, const char* text, const char* textSuffix, State<int32_t>* value,
                           int32_t min, int32_t max, int32_t step);
void addMenuElementButton(screenAction* menu, const char* text, CallbackType callback);

#endif //ESP32_MOTIONCONTROL_CREATEMENU_H
