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

// ============================================================
// WebUIManager.h
// ============================================================
#ifndef WEB_UI_MANAGER_H
#define WEB_UI_MANAGER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Images.h"

class WebUIManager {
public:
    void begin();
    void update();
    void broadcastState();

private:
    AsyncWebServer server{80};
    AsyncWebSocket ws{"/ws"};

    void setupRoutes();
    void onWsEvent(AsyncWebSocketClient* client, AwsEventType type,
                   void* arg, uint8_t* data, size_t len);
    void handleApiControl(AsyncWebServerRequest* request, uint8_t* data, size_t len);

    // HTML با base64 تصاویر
    String getIndexedHtml();

    static const char INDEX_HTML_TEMPLATE[] PROGMEM;
};

extern WebUIManager webUI;

#endif