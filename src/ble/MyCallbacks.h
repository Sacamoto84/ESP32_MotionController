//
// Created by Ivan on 19.03.2025.
//

#ifndef MYCALLBACKS_H
#define MYCALLBACKS_H

#include <Arduino.h>

#include <BLEServer.h>
#include <SimpleCLI.h>

extern SimpleCLI cli;

class MyCallbacks : public BLECharacteristicCallbacks
{

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0)
        {
            Serial2.print("BLE прием:");
            Serial2.println(value.c_str());
            cli.parse(value.c_str());
        }
    }
};

#endif //MYCALLBACKS_H
