//
// Created by Ivan on 19.03.2025.
//

#ifndef MYCALLBACKS_FIXED_H
#define MYCALLBACKS_FIXED_H

#include <Arduino.h>

#include <BLEServer.h>
#include <SimpleCLI.h>
#include "global.h"

extern SimpleCLI cli;

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        String value = pCharacteristic->getValue();

        if (value.length() > 0)
        {
            timber.i("BLE прием:");
            timber.i(value);
            cli.parse(value.c_str());
        }
    }
};

#endif //MYCALLBACKS_FIXED_H
