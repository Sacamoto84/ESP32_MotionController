#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <EncButton.h>
#include "TFT_color.h"

#include "global.h"

#include "screen/menuTypedef.h"

#include "./screen/createMenu.h"

#include "Roboto_Medium_en_ru_18.h"
#include "Roboto_Medium_en_ru_20.h"
#include "Roboto_Medium_en_ru_24.h"
#include "Roboto_Medium_en_ru_26.h"

#define AA_FONT_SMALL _acRoboto_Medium_en_ru_20

extern EncButton eb;

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// uint16 color = ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);

static bool updateLcd = true;
TaskHandle_t TaskLcd;
enum CurrenScreen currentScreen = MAIN;


// Настройки PWM для подсветки
const int backlightPin = 38;        // твой пин
const int pwmChannel   = 0;         // любой канал от 0 до 7 (ESP32-S3 имеет 8 каналов LEDC)
const int pwmFreq      = 5000;      // 5 кГц — комфортная частота, без мерцания
const int pwmResolution = 8;        // 8 бит → яркость от 0 до 255




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
    tft.fillScreen(0);
    tft.setTextColor(WHITE, BLACK);
    // tft.drawString("TFT_eSPI example", 30, 30, 2);

    //pinMode(38, OUTPUT);
    //digitalWrite(38, HIGH);

    ledcAttach(38, 5000, 8);   // пин, частота (Гц), разрешение (бит)
    ledcWrite(38, 200);

    // Настраиваем PWM на пине 38
    //ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    //ledcAttachPin(backlightPin, pwmChannel);

    tft.loadFont(AA_FONT_SMALL); // Must load the font first

    createMenuContinuous();
    createMenuConfig();
    createMenuVibro();

    screenAction *s = &menuContinuous;

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


        //tft.fillScreen(BLACK);
        //tft.drawLine(0, 64, 239,64, YELLOW);

        //tft.drawCircle(120,64, 10, YELLOW);
        //tft.drawPixel(16,16, YELLOW);

        Screen(s);

        delay(5);
    }
}

void update() {
    updateLcd = true;
}

void Screen(screenAction *screen) {

    eb.tick();

    uint16_t colorBg = screen->colorBg;//RGB565(0, 0, 128);

    // scrollbar
    float sbPercent = (float) (screen->ITEMS_WINDOW) / (float) screen->ITEMS_COUNT;
    float sbMaxHeight = 170;                                // Максимальная высота всего скролл бар
    float sbActiveHeight = (float) sbMaxHeight * sbPercent; // Высота активной части
    int sbW = 3;                                            // Ширина
    uint16_t sbColor = White;                               // Цвет активной части
    uint16_t sbColorBg = Black;                             // Цвет фона
    int sbX = 320 - 3;
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
        tft.drawRect(0, 0, 320, 170, Green);

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

    //delay(5);
}



