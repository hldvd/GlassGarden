/*
------------------------------------------------------------
GlassGarden

File : SensorManager.cpp

وظیفه:
خواندن دما و رطوبت از سنسور DHT
و ذخیره در StateManager

همچنین ثبت زمان آخرین خواندن معتبر
(state.lastValidSensorReadMs) که توسط
AutomationManager برای تشخیص Safe Mode
استفاده می‌شود.

Version : 1.1.0
------------------------------------------------------------
*/

#include "SensorManager.h"

#include <DHT.h>

#include "../core/Config.h"
#include "../state/StateManager.h"

//------------------------------------------------------------
// فاصلهٔ زمانی بین دو خواندن (میلی‌ثانیه)
//------------------------------------------------------------

constexpr unsigned long SENSOR_READ_INTERVAL = 2500;

static DHT dht(DHT_PIN, DHT_TYPE);

SensorManager sensors;

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void SensorManager::begin()
{
    dht.begin();

    // شمارش قطعی سنسور از لحظهٔ بوت آغاز می‌شود، نه از epoch=0
    // (تا Safe Mode بلافاصله بعد از بوت فعال نشود)
    state.lastValidSensorReadMs = millis();
}

//------------------------------------------------------------
// بروزرسانی
//------------------------------------------------------------

void SensorManager::update()
{
    if (millis() - lastReadTime < SENSOR_READ_INTERVAL)
    {
        return;
    }

    lastReadTime = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println("[Sensor] Read failed");
        return;
    }

    state.temperature = t;
    state.humidity = h;
    state.lastValidSensorReadMs = millis();
}