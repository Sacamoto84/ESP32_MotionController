//
// Created by Ivan on 19.03.2025.
//

#ifndef MYSERVERCALLBACKS_H
#define MYSERVERCALLBACKS_H

#include "ble.h"

class MyServerCallbacks : public BLEServerCallbacks
{

public:
    MyServerCallbacks(bool* deviceConnected1) : deviceConnected(deviceConnected1)
    {
    }

    void onConnect(BLEServer* pServer) override
    {
        *deviceConnected = true;
        BLEDevice::startAdvertising();
        timber.i.println("BLE подключен");
    }

    void onDisconnect(BLEServer* pServer) override
    {
        *deviceConnected = false;
        timber.i.println("BLE отключен");
    }

private:
    bool* deviceConnected; // Ссылка на внешнюю переменную
};

#endif //MYSERVERCALLBACKS_H
