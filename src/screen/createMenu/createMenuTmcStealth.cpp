#include "../createMenu.h"

#include "storage.h"
#include "tmc2160Config.h"

void createMenuTmcStealth()
{
    addMenuElementButton(&menuTmcStealth, "<- Настройки", [](int) {
        currentScreen = CONFIG;
        update();
    });

    addMenuElementSwitch(&menuTmcStealth, "PWM autoscale: Вкл", "PWM autoscale: Выкл", &tmcPwmAutoscale);
    addMenuElementSwitch(&menuTmcStealth, "PWM autograd: Вкл", "PWM autograd: Выкл", &tmcPwmAutograd);
    addMenuElementEDITINT(&menuTmcStealth, "PWM_OFS: ", "", &tmcPwmOfs, 0, 255, 1);
    addMenuElementEDITINT(&menuTmcStealth, "PWM_GRAD: ", "", &tmcPwmGrad, 0, 255, 1);
    addMenuElementEDITINT(&menuTmcStealth, "PWM_FREQ: ", "", &tmcPwmFreq, 0, 3, 1);
    addMenuElementEDITINT(&menuTmcStealth, "PWM_REG: ", "", &tmcPwmReg, 0, 15, 1);
    addMenuElementEDITINT(&menuTmcStealth, "PWM_LIM: ", "", &tmcPwmLim, 0, 15, 1);
    addMenuElementDynamicText(&menuTmcStealth, tmcStatusPwmLine, true);

    addMenuElementButton(&menuTmcStealth, "Сохранить", [](int) {
        saveTmc2160Settings();
        update();
    });

    finalizeMenu(&menuTmcStealth, 7);
}
