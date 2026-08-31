#pragma once
/*
------------------------------------------------------------
GlassGarden — Web UI Manager

وظیفه:
- راه‌اندازی سرور وب محلی روی ESP32
- ارتباط real-time با WebSocket
- کنترل تجهیزات و نمایش سنسورها بدون وابستگی به Blynk

Version : 1.0.0
------------------------------------------------------------
*/

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class WebUIManager
{
public:
    void begin();
    void update();

private:
    AsyncWebServer server{80};
    AsyncWebSocket ws{"/ws"};

    void setupRoutes();
    void onWsEvent(AsyncWebSocketClient* client, AwsEventType type,
                   void* arg, uint8_t* data, size_t len);
    void broadcastState();
    void handleApiControl(AsyncWebServerRequest* request, uint8_t* data, size_t len);

    static const char INDEX_HTML[] PROGMEM;
};
extern WebUIManager webUI;