#include <Arduino.h>
#include <WiFi.h>
#include "Timber.h"
#include <ESPAsyncWebServer.h>

#include <list>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

extern Timber timber;

//WiFiServer server(80); // Создаем сервер на порту 80

auto ssid = "TP-Link_BC0C";      // Имя Wi-Fi сети
auto password = "58133514";     // Пароль

IPAddress local_IP(192, 168, 0, 200);  // Заданный IP
IPAddress gateway(192, 168, 0, 1);     // Шлюз (обычно IP роутера)
IPAddress subnet(255, 255, 255, 0);    // Маска подсети

TaskHandle_t Task1;

void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial2.printf("Client #%" PRIu32 " connected.\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial2.printf("Client #%" PRIu32 " disconnected.\n", client->id());
    } else if (type == WS_EVT_ERROR) {
        Serial2.printf("Client #%" PRIu32 " error.\n", client->id());
    } else if (type == WS_EVT_DATA) {
        Serial2.printf("Client #%" PRIu32 " len: %u\n", client->id(), len);
    } else if (type == WS_EVT_PONG) {
        Serial2.printf("Client #%" PRIu32 " pong.\n", client->id());
    } else if (type == WS_EVT_PING) {
        Serial2.printf("Client #%" PRIu32 " ping.\n", client->id());
    }
}















void wifi_loop(){
    timber.i("wifi_loop()");
    //WiFi.mode(WIFI_STA);
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);
    timber.i("Подключение к Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Timber::print(".");
    }

    timber.println("\nПодключено!");

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/close_all_ws_clients", HTTP_GET | HTTP_POST, [](AsyncWebServerRequest* request) {
       Serial.println("Closing all WebSocket clients...");
       ws.closeAll();
       request->send(200, "application/json", "{\"status\":\"all clients closed\"}");
     });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
        <html>
        <head></head>
        <script>
            let ws = new WebSocket("ws://" + window.location.host + "/ws");

            ws.addEventListener("open", (e) => {
                console.log("WebSocket connected", e);
            });

            ws.addEventListener("error", (e) => {
                console.log("WebSocket error", e);
            });

            ws.addEventListener("close", (e) => {
                console.log("WebSocket close", e);
            });

            ws.addEventListener("message", (e) => {
                console.log("WebSocket message", e);
            });

            function closeAllWsClients() {
              fetch("/close_all_ws_clients", {
                  method : "POST",
              });
            };
        </script>
        <body>
            <button onclick = "closeAllWsClients()" style = "width: 200px"> ws close all</button><p></p>
          </body>
        </html>
  )rawliteral");
    });

    server.begin();

    }

// void TaskWifiLoop()
// {
//     timber.w("TaskWifiLoop()");
//     xTaskCreatePinnedToCore(
//           wifi_loop,   /* Функция для задачи */
//           "wifi_loop", /* Имя задачи */
//           10000,          /* Размер стека */
//           NULL,           /* Параметр задачи */
//           9,              /* Приоритет */
//           &Task1,         /* Выполняемая операция */
//           0);             /* Номер ядра, на котором она должна выполняться */
//
// }