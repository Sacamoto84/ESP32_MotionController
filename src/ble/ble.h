//
// Created by Ivan on 19.03.2025.
//

#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"

extern SimpleCLI cli;
extern void cliInit();

class Ble
{
public:
    void init();
    void tick();


    void TaskBleLoop(void* parameter);

    TaskHandle_t TaskBle{};

    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    BLEClient* pClient{};
    BLEServer* pServer{};
    BLECharacteristic* pCharacteristic{};
    BLECharacteristic* pReceiveCharacteristic{};
};


#endif //BLE_H
