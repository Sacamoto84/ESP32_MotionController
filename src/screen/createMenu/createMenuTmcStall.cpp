#include "../createMenu.h"

#include "storage.h"
#include "tmc2160Config.h"

void createMenuTmcStall()
{
    addMenuElementButton(&menuTmcStall, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementEDITINT(&menuTmcStall, "SGT: ", "", &tmcSgt, -64, 63, 1);
    addMenuElementSwitch(&menuTmcStall, "SFILT: Вкл", "SFILT: Выкл", &tmcSfilt);
    addMenuElementEDITINT(&menuTmcStall, "TCOOLTHRS: ", "", &tmcTCoolThrs, 0, 1048575, 100);
    addMenuElementEDITINT(&menuTmcStall, "THIGH: ", "", &tmcTHigh, 0, 1048575, 100);
    addMenuElementEDITINT(&menuTmcStall, "SEMIN: ", "", &tmcSemin, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcStall, "SEMAX: ", "", &tmcSemax, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcStall, "SEUP: ", "", &tmcSeup, 0, 3, 1);
    addMenuElementEDITINT(&menuTmcStall, "SEDN: ", "", &tmcSedn, 0, 3, 1);
    addMenuElementSwitch(&menuTmcStall, "SEIMIN: 1/2", "SEIMIN: 1/4", &tmcSeimin);
    addMenuElementDynamicText(&menuTmcStall, tmcStatusSgLine, true);

    addMenuElementButton(&menuTmcStall, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcStall, 7);
}
