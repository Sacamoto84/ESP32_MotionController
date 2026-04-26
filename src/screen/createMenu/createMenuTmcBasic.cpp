#include "../createMenu.h"

#include "storage.h"

void createMenuTmcBasic()
{
    addMenuElementButton(&menuTmcBasic, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementSwitch(&menuTmcBasic, "Мотор: Вкл", "Мотор: Выкл", &tmcStepperEnable);
    addMenuElementSwitch(&menuTmcBasic, "Режим: StealthChop", "Режим: SpreadCycle", &tmcDriverChop);
    addMenuElementEDITINT(&menuTmcBasic, "Ток RMS: ", " мА", &tmcDriverCurrent, 100, 3100, 100);
    addMenuElementMicrostep(&menuTmcBasic, &tmcDriverMicrostep);
    addMenuElementSwitch(&menuTmcBasic, "MicroPlyer: Вкл", "MicroPlyer: Выкл", &tmcInterpolation);
    addMenuElementEDITINT(&menuTmcBasic, "TPWMTHRS: ", "", &tmcTPwmThrs, 0, 1048575, 100);
    addMenuElementEDITINT(&menuTmcBasic, "TPOWERDOWN: ", "", &tmcTPowerDown, 0, 255, 1);
    addMenuElementSwitch(&menuTmcBasic, "Fast standstill: Вкл", "Fast standstill: Выкл", &tmcFastStandstill);

    addMenuElementButton(&menuTmcBasic, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcBasic, 7);
}
