/*
------------------------------------------------------------
GlassGarden
File : SystemManager.cpp
وظیفه:
راه‌اندازی و مدیریت کل سیستم
Version : 1.1.0 (اصلاح ترتیب اجرا)
------------------------------------------------------------
*/
#include "SystemManager.h"
#include "../hardware/Hardware.h"
#include "../network/NetworkManager.h"
#include "../devices/DeviceManager.h"
#include "../state/StateManager.h"
#include "../blynk/BlynkManager.h"
#include "../sensors/SensorManager.h"
#include "../automation/AutomationManager.h"

static NetworkManager network;
static BlynkManager blynk;

void SystemManager::begin()
{
    Hardware::begin();
    state.begin();
    devices.begin();
    sensors.begin();
    automation.begin();
    network.begin();
    blynk.begin();
}

void SystemManager::update()
{
    network.update();
    devices.update();
    sensors.update();
    blynk.update();          // ← اول فرمان‌های Blynk پردازش شود
    automation.update();     // ← بعد اتوماسیون با وضعیت جدید اجرا شود
    state.update();
}
