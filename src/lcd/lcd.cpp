#include <Arduino.h>
#include <EncButton.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_color.h"
#include "global.h"
#include "screen/createMenu.h"
#include "screen/menuTypedef.h"

#include "Roboto_Medium_en_ru_18.h"
#include "Roboto_Medium_en_ru_20.h"
#include "Roboto_Medium_en_ru_24.h"
#include "Roboto_Medium_en_ru_26.h"

#define AA_FONT_SMALL _acRoboto_Medium_en_ru_20

extern EncButton eb;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

static bool updateLcd = true;
TaskHandle_t taskLcd;
enum CurrenScreen currentScreen = MAIN;

namespace
{
constexpr int kDisplayWidth = 320;
constexpr int kDisplayHeight = 170;
constexpr int kScrollBarWidth = 3;
constexpr int kScrollBarX = kDisplayWidth - kScrollBarWidth;
constexpr int kScrollBarY = 1;
constexpr int kBacklightPin = 38;
constexpr int kBacklightPwmFreq = 5000;
constexpr int kBacklightPwmResolution = 8;
constexpr int kBacklightDuty = 200;
constexpr int kRenderDelayMs = 1;

int clampIndex(int value, int minValue, int maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

int lastIndex(const screenAction *screen)
{
    return static_cast<int>(screen->ITEMS_COUNT) - 1;
}

void normalizeScreenWindow(screenAction *screen)
{
    screen->ITEMS_COUNT = static_cast<uint>(screen->items.size());

    if (screen->ITEMS_COUNT == 0)
    {
        screen->ITEMS_WINDOW = 0;
        screen->line = 0;
        screen->indexStartWindow = 0;
        screen->indexEndWindow = -1;
        return;
    }

    const int last = lastIndex(screen);

    if (screen->ITEMS_WINDOW < 1)
    {
        screen->ITEMS_WINDOW = 1;
    }
    if (screen->ITEMS_WINDOW > static_cast<int>(screen->ITEMS_COUNT))
    {
        screen->ITEMS_WINDOW = static_cast<int>(screen->ITEMS_COUNT);
    }

    screen->line = clampIndex(screen->line, 0, last);
    screen->indexStartWindow = clampIndex(screen->indexStartWindow, 0, last);

    const int maxWindowStart = last - (screen->ITEMS_WINDOW - 1);
    if (screen->indexStartWindow > maxWindowStart)
    {
        screen->indexStartWindow = maxWindowStart;
    }
    if (screen->indexStartWindow < 0)
    {
        screen->indexStartWindow = 0;
    }

    screen->indexEndWindow = screen->indexStartWindow + screen->ITEMS_WINDOW - 1;
    if (screen->indexEndWindow > last)
    {
        screen->indexEndWindow = last;
    }
}

int findSelectableIndex(const screenAction *screen, int startIndex, int direction)
{
    const int last = lastIndex(screen);
    int index = clampIndex(startIndex, 0, last);

    while (index >= 0 && index <= last)
    {
        if (!screen->items[index].skipping)
        {
            return index;
        }
        index += direction;
    }

    return screen->line;
}

void ensureSelectionVisible(screenAction *screen)
{
    if (screen->line < screen->indexStartWindow)
    {
        screen->indexStartWindow = screen->line;
    }
    else if (screen->line > screen->indexEndWindow)
    {
        screen->indexStartWindow = screen->line - (screen->ITEMS_WINDOW - 1);
    }

    normalizeScreenWindow(screen);
}

void moveSelection(screenAction *screen, int direction)
{
    if (screen->ITEMS_COUNT == 0)
    {
        return;
    }

    screen->line = findSelectableIndex(screen, screen->line + direction, direction);
    ensureSelectionVisible(screen);
    timber.i("S:%d E:%d L:%d\n", screen->indexStartWindow, screen->indexEndWindow, screen->line);
}

void configureDisplay()
{
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(0);
    tft.setTextColor(WHITE, BLACK);

    spr.setColorDepth(16);
    spr.createSprite(kDisplayWidth, kDisplayHeight);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.setTextDatum(TL_DATUM);
    spr.loadFont(AA_FONT_SMALL);
    tft.loadFont(AA_FONT_SMALL);
}

void configureBacklight()
{
    pinMode(kBacklightPin, OUTPUT);
    ledcAttach(kBacklightPin, kBacklightPwmFreq, kBacklightPwmResolution);
    ledcWrite(kBacklightPin, kBacklightDuty);
}

screenAction *activeScreen()
{
    switch (currentScreen)
    {
    case CONFIG:
        return &menuConfig;
    case VIBRO:
        return &menuVibro;
    case MAIN:
    default:
        return &menuContinuous;
    }
}

void drawScrollbar(const screenAction *screen)
{
    if (screen->ITEMS_COUNT <= screen->ITEMS_WINDOW)
    {
        return;
    }

    const float scrollPercent = static_cast<float>(screen->ITEMS_WINDOW) / static_cast<float>(screen->ITEMS_COUNT);
    const float activeHeight = static_cast<float>(kDisplayHeight) * scrollPercent;
    const int scrollOffset = static_cast<int>(
        static_cast<float>(screen->indexStartWindow) / static_cast<float>(screen->ITEMS_COUNT) * kDisplayHeight);

    spr.fillRect(kScrollBarX, kScrollBarY, kScrollBarWidth, kDisplayHeight, Black);
    spr.fillRect(kScrollBarX + 1, kScrollBarY + scrollOffset, kScrollBarWidth - 2,
                 static_cast<int32_t>(activeHeight), White);
}
} // namespace

[[noreturn]] void TaskLcdLoop(void *parameter);
void Screen(screenAction *menu);

void lcdInit()
{
    xTaskCreatePinnedToCore(TaskLcdLoop, "TaskLcdLoop", 20000, nullptr, 8, &taskLcd, 0);
}

[[noreturn]] void TaskLcdLoop(void *parameter)
{
    (void)parameter;

    configureDisplay();
    configureBacklight();

    createMenuContinuous();
    createMenuConfig();
    createMenuVibro();

    while (true)
    {
        Screen(activeScreen());
        delay(kRenderDelayMs);
    }
}

void update()
{
    updateLcd = true;
}

void Screen(screenAction *screen)
{
    eb.tick();
    normalizeScreenWindow(screen);

    if (screen->ITEMS_COUNT == 0)
    {
        return;
    }

    if (eb.turn() || updateLcd || eb.press())
    {
        updateLcd = false;

        if (eb.turn() && !screen->isSelect)
        {
            if (eb.right())
            {
                moveSelection(screen, 1);
            }

            if (eb.left())
            {
                moveSelection(screen, -1);
            }
        }

        const uint16_t colorBg = screen->colorBg;
        spr.fillRect(0, 0, kDisplayWidth, kDisplayHeight, colorBg);
        spr.drawRect(0, 0, kDisplayWidth, kDisplayHeight, Green);
        spr.setTextColor(White, colorBg);

        int16_t visibleIndex = 0;
        for (int itemIndex = screen->indexStartWindow; itemIndex <= screen->indexEndWindow; ++itemIndex)
        {
            itemAction &item = screen->items[itemIndex];
            item.colorActive = screen->colorActive;
            item.colorInactive = screen->colorInactive;
            item.colorBg = screen->colorBg;

            ITEM(screen->line, itemIndex, &item, &screen->isSelect, screen->startX,
                 static_cast<int16_t>(visibleIndex * screen->height + screen->startY));
            visibleIndex += 1;
        }

        drawScrollbar(screen);
        spr.pushSprite(0, 0);
    }
}
