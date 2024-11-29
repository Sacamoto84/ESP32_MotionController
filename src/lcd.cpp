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

screenAction menu0;

void screen0(screenAction *menu);

void createMenu0();

void lcdInit() {


    xTaskCreatePinnedToCore(
            TaskLcdLoop,    /* Функция для задачи */
            "TaskLcdLoop",     /* Имя задачи */
            20000,         /* Размер стека */
            NULL,          /* Параметр задачи */
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

    createMenu0();

    while (true) {
        // xSemaphoreTake(semaphore, portMAX_DELAY);
        screen0(&menu0);
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

enum Screen0Lines {
    enable = 0,
    chop = 1,
    current = 2,
    microstep = 3,
    interpolation = 4,
    r1 = 5,
    r2 = 6,
    r3 = 7,
    r4 = 8,
};


void ITEM(int line, itemAction *item, bool *isSelect) {

    if (item->type == itemAction::SWITCH) {
        if ((line == item->index) && (eb.press())) {
            uint16_t a = item->value->get();
            a = !a;
            item->value->set(a);
        }

        auto s0 = (item->value->get()) ? item->textOn : item->textOff;

        if (line == item->index)
            Text(s0, item->x, item->y, item->colorActive, item->colorBg);
        else
            Text(s0, item->x, item->y, item->colorInactive, item->colorBg);
        return;
    }

    if ((item->type == itemAction::EDITINT) || (item->type == itemAction::EDITINTMICROSTEP)) {

        auto a = *isSelect;

        if (line == item->index) {

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

        if (line == item->index) {

            if (!a)
                Text(str11, item->x, item->y, item->colorActive, item->colorBg);
            else
                Text(str11, item->x, item->y, item->colorBg, item->colorActive, true);

        } else
            Text(str11, item->x, item->y, item->colorInactive, item->colorBg);

        return;
    }

    if (item->type == itemAction::TEXT) {
        String str11 = String(item->text);
        if (line == item->index) {
            Text(str11, item->x, item->y, item->colorActive, item->colorBg);
        } else
            Text(str11, item->x, item->y, item->colorInactive, item->colorBg);
        return;
    }




}

void screen0(screenAction *screen) {

    eb.tick();

    uint16_t colorBg = RGB565(0, 0, 128);

    // scrollbar
    float sbPercent = (float) (screen->ITEMS_WINDOW) / (float) screen->ITEMS_COUNT;
    float sbMaxHeight = 131;                                // Максимальная высота всего скролл бар
    float sbActiveHeight = (float) sbMaxHeight * sbPercent; // Высота активной части
    int sbW = 3;                                            // Ширина
    uint16_t sbColor = White;                               // Цвет активной части
    uint16_t sbColorBg = Black;                             // Цвет фона
    int sbX = 239 - 5;
    int sbY = 1;
    float sbOffsetY = 0.0f;

    if (eb.turn() || updateLcd || eb.press()) {
        updateLcd = false;

        if (eb.turn() && !screen->isSelect) {
            if (eb.right()) {
                screen->line++;

                if (screen->items[screen->line].scipping){
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

                if (screen->items[screen->line].scipping){
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

            screen->items[i].x = menu0.startX;
            screen->items[i].y = ii * menu0.height + menu0.startY;
            screen->items[i].colorActive = screen->colorActive;
            screen->items[i].colorInactive = screen->colorInactive;
            screen->items[i].colorBg = screen->colorBg;
            ITEM(screen->line, &screen->items[i], &screen->isSelect);
            ii++;
        }
        tft.fillRect(sbX, sbY, sbW, (int) sbMaxHeight, sbColorBg);
        int a = (float) screen->indexStartWindow / (float) screen->ITEMS_COUNT * sbMaxHeight;
        tft.fillRect(sbX + 1, sbY + a, sbW - 2, sbActiveHeight, sbColor);
    }

    delay(5);
}


//itemAction createDefaultItemAction() {
//    return itemAction{
//            .type = itemAction::NONE,
//            .index = 0,
//            .line = nullptr,
//            //.isSelect = nullptr,
//            .value = nullptr,
//            .min = 0,
//            .max = 1,
//            .step = 1,
//            .textOn = "on",
//            .textOff = "off",
//            .text = "?",
//            .correction = "",
//            .x = 0,
//            .y = 0,
//            .colorActive = Yellow,
//            .colorInactive = DarkGrey,
//            .colorBg = DarkGrey,
//    };
//};

void createMenu0() {

    itemAction actions;// = createDefaultItemAction();
    actions.type = itemAction::SWITCH;
    actions.index = 0;
    actions.line = &menu0.line;
    //actions0.isSelect = &menu0.isSelect,
    actions.value = &tmcDriverEnable;
    actions.textOn = "Мотор: Вкл";
    actions.textOff = "Мотор: Выкл";

            menu0.addMenuAction(actions);


    actions.type = itemAction::SWITCH;
    actions.index = 1;
    actions.line = &menu0.line;
    //actions1.isSelect = &menu0.isSelect,
    actions.value = &tmcDriverChop;
    actions.textOn = "Режим: StealthChop";
    actions.textOff = "Режим: SpreadCycle";
            menu0.addMenuAction(actions);

    //itemAction actions2;// = createDefaultItemAction();
    actions.type = itemAction::EDITINT;
    actions.index = 2,
    actions.line = &menu0.line;
    //actions2.isSelect = &menu0.isSelect,
    actions.value = &tmcDriverCurrent;
    actions.text = "Ток: ";
    actions.min = 100;
    actions.max = 3100;
    actions.step = 100;
    menu0.addMenuAction(actions);

    itemAction actions3;// = createDefaultItemAction();
    actions.type = itemAction::EDITINTMICROSTEP;
    actions.index = 3;
    actions.line = &menu0.line;
    //actions3.isSelect = &menu0.isSelect,
    actions.value = &tmcDriverMicrostep;
    actions.text = "Микрошаг: ";
    actions.min = 1;
    actions.max = 256;
    actions.step = 1;
    menu0.addMenuAction(actions);

    //itemAction actions4;// = createDefaultItemAction();
    actions.type = itemAction::SWITCH;
    actions.index = 4;
    actions.line = &menu0.line;
    //actions4.isSelect = &menu0.isSelect,
    actions.value = &tmcDriverInterpolation;
    actions.textOn = "Interpolation: Вкл";
    actions.textOff = "Interpolation: Выкл";
            menu0.addMenuAction(actions);

    //itemAction actions;// = createDefaultItemAction();
    actions.type = itemAction::TEXT;
    actions.index = 5;
    actions.line = &menu0.line;
            //actions4.isSelect = &menu0.isSelect,
    actions.text = "Текст0";
    actions.scipping = true;
    menu0.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.index = 6;
    actions.line = &menu0.line;
            //actions4.isSelect = &menu0.isSelect,
    actions.text = "Текст1";

    actions.scipping = false;
    menu0.addMenuAction(actions);
    ///////////////////////////////////
    actions.type = itemAction::TEXT;
    actions.index = 7;
    actions.line = &menu0.line;
            //actions4.isSelect = &menu0.isSelect,
    actions.text = "Текст2";
            actions.scipping = false;
            menu0.addMenuAction(actions);

    ///////////////////////////////////
    menu0.ITEMS_COUNT = 8;
    menu0.ITEMS_WINDOW = 5;
    menu0.indexEndWindow = menu0.ITEMS_WINDOW - 1;

}

