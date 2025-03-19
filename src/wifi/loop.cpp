#include <Arduino.h>
#include <WiFi.h>
#include "Timber.h"

extern Timber timber;

WiFiServer server(80); // Создаем сервер на порту 80

auto ssid = "TP-Link_BC0C";      // Имя Wi-Fi сети
auto password = "58133514";     // Пароль

IPAddress local_IP(192, 168, 0, 200);  // Заданный IP
IPAddress gateway(192, 168, 0, 1);     // Шлюз (обычно IP роутера)
IPAddress subnet(255, 255, 255, 0);    // Маска подсети

TaskHandle_t Task1;

void wifi_loop(void *parameter){
    timber.i("wifi_loop()");
    //WiFi.mode(WIFI_STA);
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);
    timber.i("Подключение к Wi-Fi");

    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Timber::print(".");
    }

    timber.println("\nПодключено!");

    server.begin(); // Запускаем сервер

    while (true)
    {

        WiFiClient client = server.available(); // Ожидаем клиента

        if (client) {  // Если клиент подключился
            timber.println("Клиент подключился!");

            while (client.connected()) {
                if (client.available()) {
                    String message = client.readStringUntil('\n'); // Читаем строку до \n
                    timber.w("Получено: ");
                    timber.i(message.c_str());

                    // Отправляем ответ клиенту
                    client.println("ESP32 принял данные!");
                }else
                {
                    delay(10);
                }
            }
            client.stop(); // Закрываем соединение
            timber.w("Клиент отключился.");
        }
        else
        {
            delay(10);
        }
    }

}

void TaskWifiLoop()
{
    timber.w("TaskWifiLoop()");
    xTaskCreatePinnedToCore(
          wifi_loop,   /* Функция для задачи */
          "wifi_loop", /* Имя задачи */
          10000,          /* Размер стека */
          NULL,           /* Параметр задачи */
          2,              /* Приоритет */
          &Task1,         /* Выполняемая операция */
          0);             /* Номер ядра, на котором она должна выполняться */

}