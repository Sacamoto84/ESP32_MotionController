#include "../createMenu.h"

#include "storage.h"
#include "tmc2160Config.h"

void createMenuTmcCurrent()
{
    addMenuElementButton(&menuTmcCurrent, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementDynamicText(&menuTmcCurrent, tmcStatusRmsCurrentLine, true);
    addMenuElementDynamicText(&menuTmcCurrent, tmcStatusCurrentRegsLine, true);
    addMenuElementEDITINT(&menuTmcCurrent, "IHOLD: ", "", &tmcIhold, 0, 31, 1);
    addMenuElementEDITINT(&menuTmcCurrent, "IHOLDDELAY: ", "", &tmcIholdDelay, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcCurrent, "Freewheel: ", "", &tmcFreewheel, 0, 3, 1);
    addMenuElementDynamicText(&menuTmcCurrent, tmcStatusCsActualLine, true);

    addMenuElementButton(&menuTmcCurrent, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcCurrent, 7);
}
