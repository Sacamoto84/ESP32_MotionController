#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEClient.h>
#include <SimpleCLI.h>

extern SimpleCLI cli;
extern void cliInit(void);

TaskHandle_t TaskBle;

BLEClient *pClient;
BLEServer *pServer;
BLECharacteristic *pCharacteristic;
BLECharacteristic *pReceiveCharacteristic;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void BLE_tick(void);

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    BLEDevice::startAdvertising();
    Serial2.println("BLE подключен");
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial2.println("BLE отключен");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{

  void onWrite(BLECharacteristic *pCharacteristic)
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

void TaskBleLoop(void *parameter)
{
  BLEDevice::init("MyESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

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
    BLE_tick();
  }
}

void BLE_init()
{
  xTaskCreatePinnedToCore(
      TaskBleLoop,   /* Функция для задачи */
      "TaskBleLoop", /* Имя задачи */
      10000,         /* Размер стека */
      NULL,          /* Параметр задачи */
      1,             /* Приоритет */
      &TaskBle,      /* Выполняемая операция */
      0);            /* Номер ядра, на котором она должна выполняться */
}

void BLE_tick()
{
  if (deviceConnected)
  { // Проверяем, есть ли подключенный клиент
    String data = "Time: " + String(millis() / 1000) + "s";
    pCharacteristic->setValue(data.c_str()); // Устанавливаем новое значение
    pCharacteristic->notify();               // Отправляем уведомление
    Serial.println("Data sent: " + data);    // Лог для отладки
    Serial2.println("Data sent: " + data);   // Лог для отладки
    delay(5000);                             // Задержка
  }
  delay(100);
}