#include "../createMenu.h"

#include "storage.h"
#include "tmc2160Config.h"

void createMenuTmcDcStep()
{
    addMenuElementButton(&menuTmcDcStep, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementText(&menuTmcDcStep, "Эксперимент: DCEN/DCO", true);
    addMenuElementEDITINT(&menuTmcDcStep, "VDCMIN: ", "", &tmcVdcMin, 0, 1048575, 100);
    addMenuElementEDITINT(&menuTmcDcStep, "DC_TIME: ", "", &tmcDcTime, 0, 1023, 1);
    addMenuElementEDITINT(&menuTmcDcStep, "DC_SG: ", "", &tmcDcSg, 0, 255, 1);
    addMenuElementDynamicText(&menuTmcDcStep, tmcStatusLostStepsLine, true);

    addMenuElementButton(&menuTmcDcStep, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcDcStep, 6);
}
