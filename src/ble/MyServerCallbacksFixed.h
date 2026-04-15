//
// Created by Ivan on 19.03.2025.
//

#ifndef MYSERVERCALLBACKS_FIXED_H
#define MYSERVERCALLBACKS_FIXED_H

#include "ble.h"
#include "global.h"

class MyServerCallbacks : public BLEServerCallbacks
{
public:
    explicit MyServerCallbacks(bool *deviceConnected1) : deviceConnected(deviceConnected1)
    {
    }

    void onConnect(BLEServer *pServer) override
    {
        *deviceConnected = true;
        BLEDevice::startAdvertising();
        timber.i("BLE подключен");
    }

    void onDisconnect(BLEServer *pServer) override
    {
        *deviceConnected = false;
        timber.i("BLE отключен");
    }

private:
    bool *deviceConnected;
};

#endif //MYSERVERCALLBACKS_FIXED_H
