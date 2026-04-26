#include "../createMenu.h"

#include "tmc2160Config.h"

void createMenuTmcDiag()
{
    addMenuElementButton(&menuTmcDiag, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementDynamicText(&menuTmcDiag, tmcStatusDrvLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusGstatLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusIoLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusLoadLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusSgLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusCsActualLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusPwmLine, true);
    addMenuElementDynamicText(&menuTmcDiag, tmcStatusLostStepsLine, true);

    addMenuElementButton(&menuTmcDiag, "Очистить GSTAT", [](int) {
        clearTmc2160Status();
        update();
    });
    addMenuElementButton(&menuTmcDiag, "Обновить", [](int) { update(); });

    finalizeMenu(&menuTmcDiag, 7);
}
