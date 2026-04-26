#include "../createMenu.h"

#include "storage.h"

namespace
{
void openScreen(CurrenScreen screen)
{
    currentScreen = screen;
    update();
}
}

void createMenuConfig()
{
    addMenuElementButton(&menuConfig, "<- Назад", [](int) {
        closeConfigScreen();
    });

    addMenuElementButton(&menuConfig, "Основные >", [](int) { openScreen(TMC_BASIC); });
    addMenuElementButton(&menuConfig, "Ток/покой >", [](int) { openScreen(TMC_CURRENT); });
    addMenuElementButton(&menuConfig, "Chopper >", [](int) { openScreen(TMC_CHOPPER); });
    addMenuElementButton(&menuConfig, "StealthChop >", [](int) { openScreen(TMC_STEALTH); });
    addMenuElementButton(&menuConfig, "Stall/Cool >", [](int) { openScreen(TMC_STALL); });
    addMenuElementButton(&menuConfig, "Диагностика >", [](int) { openScreen(TMC_DIAG); });
    addMenuElementButton(&menuConfig, "dcStep >", [](int) { openScreen(TMC_DCSTEP); });
    addMenuElementButton(&menuConfig, "Эксперт >", [](int) { openScreen(TMC_EXPERT); });

    addMenuElementButton(&menuConfig, "Сохранить TMC", [](int) {
        saveTmc2160Settings();
        update();
    });
    addMenuElementButton(&menuConfig, "Перезагрузка", [](int) { esp_restart(); });

    finalizeMenu(&menuConfig, 7);
}
