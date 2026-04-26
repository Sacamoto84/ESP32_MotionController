#include "createMenu.h"

screenAction menuConfig;
screenAction menuContinuous;
screenAction menuVibro;
screenAction menuTmcBasic;
screenAction menuTmcCurrent;
screenAction menuTmcChopper;
screenAction menuTmcStealth;
screenAction menuTmcStall;
screenAction menuTmcDiag;
screenAction menuTmcExpert;
enum CurrenScreen configReturnScreen = MAIN;

namespace
{
bool isMainModeScreen(CurrenScreen screen)
{
    return screen == MAIN || screen == VIBRO;
}
}

void openConfigScreen()
{
    if (isMainModeScreen(currentScreen))
    {
        configReturnScreen = currentScreen;
    }

    currentScreen = CONFIG;
    update();
}

void closeConfigScreen()
{
    currentScreen = isMainModeScreen(configReturnScreen) ? configReturnScreen : MAIN;
    update();
}

void finalizeMenu(screenAction* menu, int itemsWindow)
{
    menu->ITEMS_COUNT = static_cast<uint>(menu->items.size());

    if (menu->ITEMS_COUNT == 0)
    {
        menu->ITEMS_WINDOW = 0;
        menu->line = 0;
        menu->indexStartWindow = 0;
        menu->indexEndWindow = -1;
        return;
    }

    if (itemsWindow < 1)
    {
        itemsWindow = 1;
    }
    if (itemsWindow > static_cast<int>(menu->ITEMS_COUNT))
    {
        itemsWindow = static_cast<int>(menu->ITEMS_COUNT);
    }

    menu->ITEMS_WINDOW = itemsWindow;
    menu->line = 0;
    while (menu->line < static_cast<int>(menu->ITEMS_COUNT) - 1 && menu->items[menu->line].skipping)
    {
        menu->line += 1;
    }

    menu->indexStartWindow = 0;
    menu->indexEndWindow = menu->ITEMS_WINDOW - 1;
}
