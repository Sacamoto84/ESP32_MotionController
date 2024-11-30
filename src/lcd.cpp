#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <EncButton.h>
#include "TFT_color.h"

#include "global.h"

#include "menuTypedef.h"

#include "./screen/createMenu.h"

#include "Roboto_Medium_en_ru_18.h"
#include "Roboto_Medium_en_ru_20.h"
#include "Roboto_Medium_en_ru_24.h"
#include "Roboto_Medium_en_ru_26.h"

#define AA_FONT_SMALL _acRoboto_Medium_en_ru_20


extern EncButton eb;

// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   15  // Chip select control pin
// #define TFT_DC    2  // Data Command control pin
// #define TFT_RST   4  // Reset pin (could connect to RST pin)
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// uint16 color = ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);

static bool updateLcd = true;
TaskHandle_t TaskLcd;
enum CurrenScreen currentScreen = MAIN;


[[noreturn]] void TaskLcdLoop(void *parameter);

void Screen(screenAction *menu);




void lcdInit() {

    xTaskCreatePinnedToCore(
            TaskLcdLoop,    /* Функция для задачи */
            "TaskLcdLoop",     /* Имя задачи */
            20000,         /* Размер стека */
            nullptr,          /* Параметр задачи */
            8,                /* Приоритет */
            &TaskLcd,     /* Выполняемая операция */
            0);                /* Номер ядра, на котором она должна выполняться */

}

[[noreturn]] void TaskLcdLoop(void *parameter) {
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(0x1234);
    tft.setTextColor(WHITE, BLACK);
    // tft.drawString("TFT_eSPI example", 30, 30, 2);

    tft.loadFont(AA_FONT_SMALL); // Must load the font first

    createMenuContinuous();
    createMenuConfig();
    createMenuVibro();

    screenAction *s;

    while (true) {
        switch (currentScreen) {
            case MAIN:
                s = &menuContinuous;
                break;

            case CONFIG:
                s = &menuConfig;
                break;

            case VIBRO:
                s = &menuVibro;
                break;
        }

        Screen(s);
        delay(1);
    }
}

void update() {
    updateLcd = true;
}

void Text(const String& text, int16_t x = 0, int16_t y = 0, uint16_t colorText = 0, uint16_t colorBg = 0xFFFF, bool bgfill = false) {
    tft.setTextColor(colorText, colorBg, bgfill);
    tft.setCursor(x, y);
    tft.println(text);
}

void ITEM(int line, int index, itemAction *item, bool *isSelect, int16_t x, int16_t y) {

    if (item->type == itemAction::SWITCH) {
        //Serial2.println("item->type == itemAction::SWITCH");

        if ((line == index) && (eb.press())) {
            uint16_t a = item->value->get();
            a = !a;
            item->value->set(a);
        }

        auto s0 = (item->value->get()) ? &item->textOn : &item->textOff;

        //Timber.i( item->textOn);
        //Timber.i(item->textOff);
        //Timber.i(s0.c_str());

        if (line == index)
            Text(*s0, x, y, item->colorActive, item->colorBg);
        else
            Text(*s0, x, y, item->colorInactive, item->colorBg);

        return;
    }

    if ((item->type == itemAction::EDITINT) || (item->type == itemAction::EDITINTMICROSTEP)) {
        //Serial2.println("item->type == itemAction::EDITINT EDITINTMICROSTEP");
        auto a = *isSelect;

        if (line == index) {

            if (eb.press()) {
                a = !a;
                *isSelect = a;
            }

            if (a) {

                if (item->type == itemAction::EDITINT) {
                    if (eb.right()) {
                        uint16_t t = item->value->get() + item->step;
                        if (t >= item->max)
                            t = item->max;
                        item->value->set(t);
                    }
                    if (eb.left()) {
                        uint16_t t = item->value->get() - item->step;
                        if (t <= item->min)
                            t = item->min;
                        item->value->set(t);
                    }


                }

                if (item->type == itemAction::EDITINTMICROSTEP) {
                    if (eb.right()) {
                        uint16_t t = item->value->get() * 2;
                        if (t >= item->max)
                            t = item->max;
                        item->value->set(t);
                    }
                    if (eb.left()) {
                        uint16_t t = item->value->get() / 2;
                        if (t <= item->min)
                            t = item->min;
                        item->value->set(t);
                    }
                }

            }
        }

        String str11 = String(item->text) + item->value->get() + item->testSuffix;

        if (line == index) {

            if (!a)
                Text(str11, x, y, item->colorActive, item->colorBg);
            else
                Text(str11, x, y, item->colorBg, item->colorActive, true);

        } else
            Text(str11, x, y, item->colorInactive, item->colorBg);

        return;
    }




    if (item->type == itemAction::EDITINT_I32) {
        //Serial2.println("item->type == itemAction::EDITINT EDITINTMICROSTEP");
        auto a = *isSelect;

        if (line == index) {

            if (eb.press()) {
                a = !a;
                *isSelect = a;
            }

            if (a) {

                    if (eb.right()) {
                        int32_t t = item->value_i32->get() + item->step;
                        if (t >= item->max)
                            t = item->max;
                        item->value_i32->set(t);
                    }
                    if (eb.left()) {
                        int32_t t = item->value_i32->get() - item->step;
                        if (t <= item->min)
                            t = item->min;
                        item->value_i32->set(t);
                    }

            }
        }

        String str11 = String(item->text) + item->value_i32->get() + item->testSuffix;

        if (line == index) {

            if (!a)
                Text(str11, x, y, item->colorActive, item->colorBg);
            else
                Text(str11, x, y, item->colorBg, item->colorActive, true);

        } else
            Text(str11, x, y, item->colorInactive, item->colorBg);

        return;
    }















    if (item->type == itemAction::TEXT) {
        //Serial2.println("item->type == itemAction::TEXT");
        String str11 = String(item->text);
        if (line == index) {
            Text(str11, x, y, item->colorActive, item->colorBg);
        } else
            Text(str11, x, y, item->colorInactive, item->colorBg);
        return;
    }

    if (item->type == itemAction::BUTTON) {

        //Serial2.printf("item->type == itemAction::BUTTON line:%d index:%d\n", line, index);

        if (line == index)
            Text(item->text, x, y, item->colorActive, item->colorBg);
        else
            Text(item->text, x, y, item->colorInactive, item->colorBg);

        if ((line == index) && eb.press()) {
            eb.clear();
            //Serial2.printf("item->type == itemAction::BUTTON press() line:%d index:%d\n", line, index);
            item->executeCallback(0);
        }
        return;
    }


}

void Screen(screenAction *screen) {

    eb.tick();

    uint16_t colorBg = RGB565(0, 0, 128);

    // scrollbar
    float sbPercent = (float) (screen->ITEMS_WINDOW) / (float) screen->ITEMS_COUNT;
    float sbMaxHeight = 132;                                // Максимальная высота всего скролл бар
    float sbActiveHeight = (float) sbMaxHeight * sbPercent; // Высота активной части
    int sbW = 3;                                            // Ширина
    uint16_t sbColor = White;                               // Цвет активной части
    uint16_t sbColorBg = Black;                             // Цвет фона
    int sbX = 239 - 3;
    int sbY = 1;

    if (eb.turn() || updateLcd || eb.press()) {
        updateLcd = false;

        if (eb.turn() && !screen->isSelect) {
            if (eb.right()) {
                screen->line++;

                if (screen->items[screen->line].skipping) {
                    screen->line++;
                }

                if (screen->line > screen->ITEMS_COUNT - 1)
                    screen->line = (int)screen->ITEMS_COUNT - 1;

                if (screen->line > screen->indexEndWindow) {
                    screen->indexEndWindow = screen->line;
                    screen->indexStartWindow = screen->line - (screen->ITEMS_WINDOW - 1);
                    if (screen->indexStartWindow < 0) screen->indexStartWindow = 0;
                }

                Serial2.printf("S:%d E:%d L:%d\n", screen->indexStartWindow, screen->indexEndWindow, screen->line);
            }

            if (eb.left()) {

                screen->line--;

                if (screen->items[screen->line].skipping) {
                    screen->line--;
                }

                if (screen->line < 0) {
                    screen->line = 0;
                }

                if (screen->line < screen->indexStartWindow) {
                    screen->indexStartWindow = screen->line;
                    screen->indexEndWindow = screen->line + screen->ITEMS_WINDOW - 1;
                }

                Serial2.printf("S:%d E:%d L:%d\n", screen->indexStartWindow, screen->indexEndWindow, screen->line);
            }
        }

        tft.fillScreen(colorBg);
        tft.drawRect(0, 0, 240, 135, Green);

        tft.setTextColor(WHITE, BLACK);
        tft.setTextColor(White, colorBg);
        // tft.drawString(">", 2, line * height);

        int16_t ii = 0;

        for (int i = screen->indexStartWindow; i <= screen->indexEndWindow; i++) {
            screen->items[i].colorActive = screen->colorActive;
            screen->items[i].colorInactive = screen->colorInactive;
            screen->items[i].colorBg = screen->colorBg;
            ITEM(screen->line, i, &screen->items[i], &screen->isSelect, screen->startX, (int16_t)(ii * screen->height + screen->startY));
            ii++;
        }

        if(screen->ITEMS_COUNT > screen->ITEMS_WINDOW) {
            tft.fillRect(sbX, sbY, sbW, (int) sbMaxHeight, sbColorBg);
            int a = (int) ((float) screen->indexStartWindow / (float) screen->ITEMS_COUNT * sbMaxHeight);
            tft.fillRect(sbX + 1, sbY + a, sbW - 2, (int32_t) sbActiveHeight, sbColor);
        }
    }

    delay(5);
}



