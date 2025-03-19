
#include "ble/MyCallbacks.h"
#include "ble/MyServerCallbacks.h"
#include "ble.h"

/**
 * 
 * @param parameter 
 */
void Ble::TaskBleLoop(void *parameter)
{

  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks( &deviceConnected));

  BLEService *pService = pServer->createService("0000180d-0000-1000-8000-00805f9b34fb");
  pCharacteristic = pService->createCharacteristic("00002a37-0000-1000-8000-00805f9b34fb",
                                                   BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->addDescriptor(new BLE2902());
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  pClient = BLEDevice::createClient();

  cliInit();

  while (true)
  {
    tick();
  }
}



