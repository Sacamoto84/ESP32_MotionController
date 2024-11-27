#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"
#include <Arduino.h>

#include <string>

#include <TFT_eSPI.h> // Hardware-specific library
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

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY 0x5AEB

/* some RGB color definitions                                                 */
#define Black 0x0000       /*   0,   0,   0 */
#define Navy 0x000F        /*   0,   0, 128 */
#define DarkGreen 0x03E0   /*   0, 128,   0 */
#define DarkCyan 0x03EF    /*   0, 128, 128 */
#define Maroon 0x7800      /* 128,   0,   0 */
#define Purple 0x780F      /* 128,   0, 128 */
#define Olive 0x7BE0       /* 128, 128,   0 */
#define LightGrey 0xC618   /* 192, 192, 192 */
#define DarkGrey 0x7BEF    /* 128, 128, 128 */
#define Blue 0x001F        /*   0,   0, 255 */
#define Green 0x07E0       /*   0, 255,   0 */
#define Cyan 0x07FF        /*   0, 255, 255 */
#define Red 0xF800         /* 255,   0,   0 */
#define Magenta 0xF81F     /* 255,   0, 255 */
#define Yellow 0xFFE0      /* 255, 255,   0 */
#define White 0xFFFF       /* 255, 255, 255 */
#define Orange 0xFD20      /* 255, 165,   0 */
#define GreenYellow 0xAFE5 /* 173, 255,  47 */
#define Pink 0xF81F

// uint16 color = ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);

TaskHandle_t TaskLcd;
void TaskLcdLoop(void *parameter);
void screen0();

void lcdInit()
{
    xTaskCreatePinnedToCore(
        TaskLcdLoop,   /* Функция для задачи */
        "TaskLcdLoop", /* Имя задачи */
        10000,         /* Размер стека */
        NULL,          /* Параметр задачи */
        8,             /* Приоритет */
        &TaskLcd,      /* Выполняемая операция */
        0);            /* Номер ядра, на котором она должна выполняться */
}

void TaskLcdLoop(void *parameter)
{
    vSemaphoreCreateBinary(semaphore);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(0x1234);
    tft.setTextColor(WHITE, BLACK);
    // tft.drawString("TFT_eSPI example", 30, 30, 2);

    tft.loadFont(AA_FONT_SMALL); // Must load the font first

    while (true)
    {
        // xSemaphoreTake(semaphore, portMAX_DELAY);
        screen0();

        // delay(10);
    }
}

bool updateLcd = true;

void update()
{
    updateLcd = true;
    // xSemaphoreGive(semaphore);
}

void Text(String text, int x = 0, int y = 0, uint16_t colorText = 0, uint16_t colorBg = 0xFFFF, bool bgfill = false)
{
    tft.setTextColor(colorText, colorBg, bgfill);
    tft.setCursor(x, y);
    // tft.drawString(text, x, y);
    tft.println(text);
}

enum Screen0Lines
{
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

// Определение типа функции для обработки value
typedef void (*ValueProcessor)(State<uint16_t> value);
/**
 * index - индекс в списке
 * line  - текущий индекс
 * isSelect - признак того что выбрана строка, обращается к общей &isSelect
 */
void SWITCH(int index, int line, bool *isSelect, State<uint16_t> *value, String text_on, String text_off, int x, int y, uint16_t colorActive, uint16_t colorInactve, uint16_t colorBg)
{
    if ((line == index) && (eb.press()))
    {
        uint16_t a = value->get();
        a = !a;
        value->set(a);
    }

    auto s0 = (value->get()) ? text_on : text_off;

    if (line == index)
        Text(s0, x, y, colorActive, colorBg);
    else
        Text(s0, x, y, colorInactve, colorBg);
}

void EDITINT(int index, int line, bool *isSelect, State<uint16_t> *value, uint16_t min, uint16_t max, uint16_t step, String text, int x, int y, uint16_t colorActive, uint16_t colorInactve, uint16_t colorBg, String correction = "")
{
    auto a = *isSelect;

    if (line == index)
    {

        if (eb.press())
        {
            a = !a;
            *isSelect = a;
        }

        if (a)
        {
            // Микрошаг
            if (correction == "")
            {

                if (eb.right())
                {
                    uint16_t t = value->get() + step;
                    if (t >= max)
                        t = max;
                    value->set(t);
                }
                if (eb.left())
                {
                    uint16_t t = value->get() - step;
                    if (t <= min)
                        t = min;
                    value->set(t);
                }
            }
            else
            {

                if (correction == "microstep")
                {

                    if (eb.right())
                    {
                        uint16_t t = value->get() * 2;
                        if (t >= max)
                            t = max;
                        value->set(t);
                    }
                    if (eb.left())
                    {
                        uint16_t t = value->get() / 2;
                        if (t <= min)
                            t = min;
                        value->set(t);
                    }
                }
            }
        }
    }

    String str11 = String(text) + value->get();

    if (line == index)
    {
        if (!a)
            Text(str11, x, y, Yellow, colorBg);
        else
            Text(str11, x, y, colorBg, Yellow, true);
    }
    else
        Text(str11, x, y, colorInactve, colorBg);
}

void screen0()
{

    int ITEMS_COUNT = 9;  // Количество елементов в списке
    int ITEMS_WINDOW = 5; // Количество отображаемых елементов

    eb.tick();

    static int line = 0; // Текущая строка
    static bool isSelect = false;

    uint16_t colorBg = RGB565(0, 0, 128);
    uint16_t colorInactve = DarkGrey;
    uint16_t colorActive = Yellow;

    int height = 26;
    int startY = 5;
    int startX = 6;

    static int indexStartWindow = 0;
    //int windowDelta = (ITEMS_WINDOW)-1;
    static int indexEndWindow = ITEMS_WINDOW - 1;

    // scrollbar
    float sbPercent = (float)(ITEMS_WINDOW) / (float)ITEMS_COUNT;
    float sbMaxHeight = 131;                               // Максимальная высота всего скролл бар
    float sbActiveHeight = (float)sbMaxHeight * sbPercent; // Высота активной части
    int sbW = 3;                                           // Ширина
    uint16_t sbColor = White;                              // Цвет активной части
    uint16_t sbColorBg = Black;                            // Цвет фона
    int sbX = 239 - 5;
    int sbY = 1;
    float sbOffsetY = 0.0f;

    if (eb.turn() || (updateLcd == true) || eb.press())
    {
        updateLcd = false;

        if (eb.turn() && !isSelect)
        {
            if (eb.right())
            {
                line++;
                if (line > ITEMS_COUNT - 1)
                    line = ITEMS_COUNT - 1;

                if (line > indexEndWindow)
                {
                    indexEndWindow = line;
                    indexStartWindow = line - (ITEMS_WINDOW - 1);
                    if(indexStartWindow < 0) indexStartWindow = 0;
                }

                Serial2.printf("S:%d E:%d L:%d\n", indexStartWindow, indexEndWindow, line);
            }
            if (eb.left())
            {
                line--;
                if (line < 0)
                    line = 0;

                if (line < indexStartWindow)
                {
                    indexStartWindow = line;
                    indexEndWindow = line + ITEMS_WINDOW - 1;
                }

                Serial2.printf("S:%d E:%d L:%d\n", indexStartWindow, indexEndWindow, line);
            }
        }

        tft.fillScreen(colorBg);
        tft.drawRect(0, 0, 240, 135, Green);

        tft.setTextColor(WHITE, BLACK);
        tft.setTextColor(White, colorBg);
        // tft.drawString(">", 2, line * height);

        int ii = 0;
        for (int i = indexStartWindow; i <= indexEndWindow; i++)
        {

            if (i == Screen0Lines::enable)
                SWITCH(Screen0Lines::enable, line, &isSelect, &tmcDriverEnable, "Мотор: Вкл", "Мотор: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::chop)
                SWITCH(Screen0Lines::chop, line, &isSelect, &tmcDriverChop, "Режим: StealthChop", "Режим: SpreadCycle", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::current)
                EDITINT(Screen0Lines::current, line, &isSelect, &tmcDriverCurrent, 100, 4300, 100, "Ток: ", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::microstep)
                EDITINT(Screen0Lines::microstep, line, &isSelect, &tmcDriverMicrostep, 1, 256, 1, "Микрошаг: ", startX, ii * height + startY, colorActive, colorInactve, colorBg, "microstep");

            if (i == Screen0Lines::interpolation)
                SWITCH(Screen0Lines::interpolation, line, &isSelect, &tmcDriverInterpolation, "Interpolation: Вкл", "Interpolation: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::r1)
                SWITCH(Screen0Lines::r1, line, &isSelect, &tmcDriverInterpolation, "r1: Вкл", "r1: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);
            
            if (i == Screen0Lines::r2)
                SWITCH(Screen0Lines::r2, line, &isSelect, &tmcDriverInterpolation, "r2: Вкл", "r2: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::r3)
                SWITCH(Screen0Lines::r3, line, &isSelect, &tmcDriverInterpolation, "r3: Вкл", "r3: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            if (i == Screen0Lines::r4)
                SWITCH(Screen0Lines::r4, line, &isSelect, &tmcDriverInterpolation, "r4: Вкл", "r4: Выкл", startX, ii * height + startY, colorActive, colorInactve, colorBg);

            ii++;
        }

        tft.fillRect(sbX, sbY, sbW, sbMaxHeight, sbColorBg);
        int a = (float)indexStartWindow / (float)ITEMS_COUNT * sbMaxHeight;
        tft.fillRect(sbX + 1, sbY + a, sbW - 2, sbActiveHeight, sbColor);
    }
    delay(5);
}