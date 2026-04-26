#ifndef ESP32_MOTIONCONTROL_CREATEMENU_H
#define ESP32_MOTIONCONTROL_CREATEMENU_H

#include "global.h"
#include "menuTypedef.h"

enum CurrenScreen {
    MAIN,
    CONFIG,
    VIBRO,
    TMC_BASIC,
    TMC_CURRENT,
    TMC_CHOPPER,
    TMC_STEALTH,
    TMC_STALL,
    TMC_DIAG,
    TMC_EXPERT,
};

extern screenAction menuConfig;
extern screenAction menuContinuous;
extern screenAction menuVibro;
extern screenAction menuTmcBasic;
extern screenAction menuTmcCurrent;
extern screenAction menuTmcChopper;
extern screenAction menuTmcStealth;
extern screenAction menuTmcStall;
extern screenAction menuTmcDiag;
extern screenAction menuTmcExpert;

extern enum CurrenScreen currentScreen;
extern enum CurrenScreen configReturnScreen;

void finalizeMenu(screenAction* menu, int itemsWindow);
void openConfigScreen();
void closeConfigScreen();
void createMenuConfig();
void createMenuContinuous();
void createMenuVibro();
void createMenuTmcBasic();
void createMenuTmcCurrent();
void createMenuTmcChopper();
void createMenuTmcStealth();
void createMenuTmcStall();
void createMenuTmcDiag();
void createMenuTmcExpert();

void addMenuElementText(screenAction* menu, const char* text, bool skipping = false);
void addMenuElementDynamicText(screenAction* menu, TextCallbackType textCallback, bool skipping = false);
void addMenuElementSwitch(screenAction* menu, const char* textOn, const char* textOff, State<int32_t>* value);
void addMenuElementEDITINT(screenAction* menu, const char* text, const char* textSuffix, State<int32_t>* value,
                           int32_t min, int32_t max, int32_t step);
void addMenuElementEditFloat(screenAction* menu, const char* text, const char* textSuffix, State<float>* value,
                           float min, float max, float step);
void addMenuElementButton(screenAction* menu, const char* text, CallbackType callback);
void addMenuElementMicrostep(screenAction* menu, State<int32_t>* value);

#endif //ESP32_MOTIONCONTROL_CREATEMENU_H
