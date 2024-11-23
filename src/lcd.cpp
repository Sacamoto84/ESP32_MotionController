#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"
#include <Arduino.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <TMCStepper.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>

SemaphoreHandle_t semaphore; // Дескриптор семафора


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
        2,             /* Приоритет */
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

    while (true)
    {
        xSemaphoreTake(semaphore, portMAX_DELAY);
        screen0();
        
        //delay(10);
    }
}

void update()
{
    xSemaphoreGive(semaphore);
}

void screen0()
{
    tft.setTextColor(WHITE, BLACK);
    
    if (tmcDriverEnable)
    {
        tft.fillScreen(0x3D6E70);
        tft.drawString("EN ON", 30, 30, 4);
    }
    else
    {
        tft.fillScreen(0x3D6E70);
        tft.drawString("EN OFF", 30, 30, 4);
    }
}
