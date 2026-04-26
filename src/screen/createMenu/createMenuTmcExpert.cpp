#include "../createMenu.h"

#include "storage.h"

void createMenuTmcExpert()
{
    addMenuElementButton(&menuTmcExpert, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementSwitch(&menuTmcExpert, "Recalibrate: Вкл", "Recalibrate: Выкл", &tmcRecalibrate);
    addMenuElementSwitch(&menuTmcExpert, "Multistep filt: Вкл", "Multistep filt: Выкл", &tmcMultistepFilt);
    addMenuElementSwitch(&menuTmcExpert, "Stop enable: Вкл", "Stop enable: Выкл", &tmcStopEnable);
    addMenuElementSwitch(&menuTmcExpert, "DIAG0 error: Вкл", "DIAG0 error: Выкл", &tmcDiag0Error);
    addMenuElementSwitch(&menuTmcExpert, "DIAG0 OTPW: Вкл", "DIAG0 OTPW: Выкл", &tmcDiag0Otw);
    addMenuElementSwitch(&menuTmcExpert, "DIAG0 stall: Вкл", "DIAG0 stall: Выкл", &tmcDiag0Stall);
    addMenuElementSwitch(&menuTmcExpert, "DIAG1 stall: Вкл", "DIAG1 stall: Выкл", &tmcDiag1Stall);
    addMenuElementSwitch(&menuTmcExpert, "DIAG1 index: Вкл", "DIAG1 index: Выкл", &tmcDiag1Index);
    addMenuElementSwitch(&menuTmcExpert, "DIAG1 onstate: Вкл", "DIAG1 onstate: Выкл", &tmcDiag1Onstate);
    addMenuElementSwitch(&menuTmcExpert, "DIAG1 skipped: Вкл", "DIAG1 skipped: Выкл", &tmcDiag1StepsSkipped);
    addMenuElementSwitch(&menuTmcExpert, "DIAG0: уровень", "DIAG0: откр.сток", &tmcDiag0PushPull);
    addMenuElementSwitch(&menuTmcExpert, "DIAG1: уровень", "DIAG1: откр.сток", &tmcDiag1PushPull);
    addMenuElementEDITINT(&menuTmcExpert, "S2VS level: ", "", &tmcS2vsLevel, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcExpert, "S2G level: ", "", &tmcS2gLevel, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcExpert, "Short filter: ", "", &tmcShortFilter, 0, 3, 1);
    addMenuElementSwitch(&menuTmcExpert, "Short delay: Вкл", "Short delay: Выкл", &tmcShortDelay);
    addMenuElementEDITINT(&menuTmcExpert, "BBM time: ", "", &tmcBbmTime, 0, 24, 1);
    addMenuElementEDITINT(&menuTmcExpert, "BBM clocks: ", "", &tmcBbmClks, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcExpert, "OT select: ", "", &tmcOtSelect, 0, 3, 1);
    addMenuElementEDITINT(&menuTmcExpert, "DRV strength: ", "", &tmcDrvStrength, 0, 3, 1);
    addMenuElementEDITINT(&menuTmcExpert, "ISENSE filter: ", "", &tmcFiltIsense, 0, 3, 1);

    addMenuElementButton(&menuTmcExpert, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcExpert, 7);
}
