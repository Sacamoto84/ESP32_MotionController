#include "../createMenu.h"

#include "storage.h"

void createMenuTmcChopper()
{
    addMenuElementButton(&menuTmcChopper, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementEDITINT(&menuTmcChopper, "TOFF: ", "", &tmcToff, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcChopper, "TBL: ", "", &tmcTbl, 0, 3, 1);
    addMenuElementEDITINT(&menuTmcChopper, "HSTRT: ", "", &tmcHstrt, 0, 7, 1);
    addMenuElementEDITINT(&menuTmcChopper, "HEND: ", "", &tmcHend, 0, 15, 1);
    addMenuElementSwitch(&menuTmcChopper, "CHM: Classic", "CHM: Spread", &tmcChm);
    addMenuElementSwitch(&menuTmcChopper, "DEDGE: Вкл", "DEDGE: Выкл", &tmcDedge);
    addMenuElementSwitch(&menuTmcChopper, "VHIGHFS: Вкл", "VHIGHFS: Выкл", &tmcVhighfs);
    addMenuElementSwitch(&menuTmcChopper, "VHIGHCHM: Вкл", "VHIGHCHM: Выкл", &tmcVhighchm);

    addMenuElementButton(&menuTmcChopper, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcChopper, 7);
}
