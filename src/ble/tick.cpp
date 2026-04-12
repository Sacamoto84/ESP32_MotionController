#include "ble.h"

void Ble::tick(void)
{
    if (deviceConnected)
    { // Проверяем, есть ли подключенный клиент
        // String data = "Time: " + String(millis() / 1000) + "s";
        report0();
        // pCharacteristic->setValue(data.c_str()); // Устанавливаем новое значение
        pCharacteristic->setValue(buf20, 4); // Устанавливаем новое значение
        //pCharacteristic->notify();           // Отправляем уведомление

        // Serial2.println("Data sent: ");    // Лог для отладки
        // Serial2.println("Data sent: " + data);   // Лог для отладки
        delay(50); // Задержка
    }
    delay(100);
}