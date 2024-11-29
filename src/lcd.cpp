#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"
#include <Arduino.h>

#include <string>

#include <TFT_eSPI.h>
#include <SPI.h>
#include <TMCStepper.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>

#include <EncButton.h>

#include "TFT_color.h"

#include "global.h"

#include "menuTypedef.h"

#include "Roboto_Medium_en_ru_18.h"
#include "Roboto_Medium_en_ru_20.h"
#include "Roboto_Medium_en_ru_24.h"
#include "Roboto_Medium_en_ru_26.h"

#define AA_FONT_SMALL _acRoboto_Medium_en_ru_24

SemaphoreHandle_t semaphore; // Дескриптор семафора

extern EncButton eb;
// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   15  // Chip select control pin
// #define TFT_DC    2  // Data Command control pin
// #define TFT_RST   4  // Reset pin (could connect to RST pin)
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// uint16 color = ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);

TaskHandle_t TaskLcd;

[[noreturn]] void TaskLcdLoop(void *parameter);

//
screenAction menuConfig;
screenAction menuMain;


void Screen(screenAction *menu);

enum CurrenScreen {
    MAIN,
    CONFIG,
} currentScreen = MAIN;


void createMenuConfig();

void createMenuMain();

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

    vSemaphoreCreateBinary(semaphore);
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(0x1234);
    tft.setTextColor(WHITE, BLACK);
    // tft.drawString("TFT_eSPI example", 30, 30, 2);

    tft.loadFont(AA_FONT_SMALL); // Must load the font first

    createMenuMain();
    createMenuConfig();

    screenAction *s;

    while (true) {

        // xSemaphoreTake(semaphore, portMAX_DELAY);
        switch (currentScreen) {
            case MAIN:
                s = &menuMain;
                break;

            case CONFIG:
                s = &menuConfig;
                break;
        }

        Screen(s);
        delay(1);
    }
}

bool updateLcd = true;

void update() {
    updateLcd = true;
    // xSemaphoreGive(semaphore);
}

void Text(String text, int x = 0, int y = 0, uint16_t colorText = 0, uint16_t colorBg = 0xFFFF, bool bgfill = false) {
    tft.setTextColor(colorText, colorBg, bgfill);
    tft.setCursor(x, y);
    // tft.drawString(text, x, y);
    tft.println(text);
}

void ITEM(int line, int index, itemAction *item, bool *isSelect, int x, int y) {

    if (item->type == itemAction::SWITCH) {
        Serial2.println("item->type == itemAction::SWITCH");

        if ((line == index) && (eb.press())) {
            uint16_t a = item->value->get();
            a = !a;
            item->value->set(a);
        }

        auto s0 = (item->value->get()) ? item->textOn : item->textOff;

        if (line == index)
            Text(s0, x, y, item->colorActive, item->colorBg);
        else
            Text(s0, x, y, item->colorInactive, item->colorBg);

        return;
    }

    if ((item->type == itemAction::EDITINT) || (item->type == itemAction::EDITINTMICROSTEP)) {
        Serial2.println("item->type == itemAction::EDITINT EDITINTMICROSTEP");
        auto a = *isSelect;

        if (line == index) {

            if (eb.press()) {
                a = !a;
                *isSelect = a;
            }

            if (a) {

                // Микрошаг
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

        String str11 = String(item->text) + item->value->get();

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
        Serial2.println("item->type == itemAction::TEXT");
        String str11 = String(item->text);
        if (line == index) {
            Text(str11, x, y, item->colorActive, item->colorBg);
        } else
            Text(str11, x, y, item->colorInactive, item->colorBg);
        return;
    }

    if (item->type == itemAction::BUTTON) {

        Serial2.printf("item->type == itemAction::BUTTON line:%d index:%d\n", line, index);

        if (line == index)
            Text(item->text, x, y, item->colorActive, item->colorBg);
        else
            Text(item->text, x, y, item->colorInactive, item->colorBg);

        if ((line == index) && eb.press()) {
            eb.clear();
            Serial2.printf("item->type == itemAction::BUTTON press() line:%d index:%d\n", line, index);
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
                    screen->line = screen->ITEMS_COUNT - 1;

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

        int ii = 0;

        for (int i = screen->indexStartWindow; i <= screen->indexEndWindow; i++) {
            screen->items[i].colorActive = screen->colorActive;
            screen->items[i].colorInactive = screen->colorInactive;
            screen->items[i].colorBg = screen->colorBg;
            ITEM(screen->line, i, &screen->items[i], &screen->isSelect, screen->startX, ii * screen->height + screen->startY);
            ii++;
        }
        tft.fillRect(sbX, sbY, sbW, (int) sbMaxHeight, sbColorBg);
        int a = (float) screen->indexStartWindow / (float) screen->ITEMS_COUNT * sbMaxHeight;
        tft.fillRect(sbX + 1, sbY + a, sbW - 2, (int32_t)sbActiveHeight, sbColor);
    }

    delay(5);
}


void createMenuMain() {

    itemAction actions;

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = "12345Мотор: Вкл";
    actions.textOff = "Мотор: Выкл";
    menuMain.addMenuAction(actions);
    Serial2.println(actions.textOn);

    Timber.i(actions.textOn.c_str());

    ///////////////////////////////////

    itemAction actions1;
    actions1.type = itemAction::BUTTON;
    //actions1.line = &menuMain.line;
    actions1.text = "Настройка";
    actions1.callback = [](int data) {
        Serial2.println("Нажата кнопка: Настройка");
        currentScreen = CONFIG;
        update();
    };
    menuMain.addMenuAction(actions1);

    //////////////////////////////////
    actions.callback = nullptr;
    //////////////////////////////////
    menuMain.ITEMS_COUNT = menuMain.items.size();
    menuMain.ITEMS_WINDOW = 5;
    menuMain.indexEndWindow = menuMain.ITEMS_WINDOW - 1;

}


void createMenuConfig() {

    itemAction actions;

    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "<- Назад";
    actions.callback = [](int data) {
        Serial2.println("Нажата кнопка: Назад");
        currentScreen = MAIN;
        update();
    };

    menuConfig.addMenuAction(actions);

    actions.callback = nullptr;

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverEnable;
    actions.textOn = "Мотор: Вкл";
    actions.textOff = "Мотор: Выкл";
    actions.skipping = false;
    menuConfig.addMenuAction(actions);

    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverChop;
    actions.textOn = "Режим: StealthChop";
    actions.textOff = "Режим: SpreadCycle";
    menuConfig.addMenuAction(actions);

    actions.type = itemAction::EDITINT;
    actions.value = &tmcDriverCurrent;
    actions.text = "Ток: ";
    actions.min = 100;
    actions.max = 3100;
    actions.step = 100;
    menuConfig.addMenuAction(actions);


    actions.type = itemAction::EDITINTMICROSTEP;
    actions.value = &tmcDriverMicrostep;
    actions.text = "Микрошаг: ";
    actions.min = 1;
    actions.max = 256;
    actions.step = 1;
    menuConfig.addMenuAction(actions);


    actions.type = itemAction::SWITCH;
    actions.value = &tmcDriverInterpolation;
    actions.textOn = "Interpolation: Вкл";
    actions.textOff = "Interpolation: Выкл";
    menuConfig.addMenuAction(actions);

    //itemAction actions;// = createDefaultItemAction();
    actions.type = itemAction::TEXT;
    actions.text = "Текст0";
    actions.skipping = true;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.text = "Текст1";
    actions.skipping = false;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::BUTTON;
    actions.text = "Button";
    actions.skipping = false;
    // Устанавливаем лямбду как коллбек
    actions.callback = [](int data) {
        Serial2.println("Нажата кнопка");
    };

    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.text = "Текст2";
    actions.skipping = false;
    actions.callback = nullptr;
    menuConfig.addMenuAction(actions);
    ///////////////////////////////////
    menuConfig.ITEMS_COUNT = menuConfig.items.size();
    menuConfig.ITEMS_WINDOW = 5;
    menuConfig.indexEndWindow = menuConfig.ITEMS_WINDOW - 1;

}

