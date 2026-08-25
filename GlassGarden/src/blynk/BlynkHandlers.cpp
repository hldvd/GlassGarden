/*
------------------------------------------------------------
GlassGarden
File : BlynkHandlers.cpp
وظیفه:
دریافت فرمان‌های اپلیکیشن Blynk و انتقال آن‌ها
به DeviceManager
Version : 1.1.1 (خطایابی کامل همه دکمه‌ها)
------------------------------------------------------------
*/
#include "../core/Config.h"
#include <Arduino.h>
#include <BlynkApiArduino.h>
#include "../core/Datastreams.h"
#include "../devices/DeviceManager.h"
#include "../state/StateManager.h"
#include "BlynkHandlersHook.h"

// فراخوانی برای جلوگیری از حذف فایل توسط لینکر
void registerBlynkHandlers()
{
    // عمداً خالی است
}

//------------------------------------------------------------
// Light — V0
//------------------------------------------------------------
BLYNK_WRITE(V0)
{
    int val = param.asInt();
    Serial.printf("[Blynk-CMD] V0 Light = %d\n", val);
    if (val) devices.lightOn();
    else     devices.lightOff();
}

//------------------------------------------------------------
// Fogger — V3
//------------------------------------------------------------
BLYNK_WRITE(V3)
{
    int val = param.asInt();
    Serial.printf("[Blynk-CMD] V3 Fogger = %d\n", val);
    if (val) devices.foggerOn();
    else     devices.foggerOff();
}

//------------------------------------------------------------
// Fan — V4
//------------------------------------------------------------
BLYNK_WRITE(V4)
{
    int val = param.asInt();
    Serial.printf("[Blynk-CMD] V4 Fan = %d\n", val);
    if (val) devices.fanOn();
    else     devices.fanOff();
}

//------------------------------------------------------------
// Pump — V5
//------------------------------------------------------------
BLYNK_WRITE(V5)
{
    int val = param.asInt();
    Serial.printf("[Blynk-CMD] V5 Pump = %d\n", val);
    if (val) devices.pumpOn();
    else     devices.pumpOff();
}

//------------------------------------------------------------
// AUTO / MANUAL — V6
//------------------------------------------------------------
BLYNK_WRITE(V6)
{
    state.autoMode = param.asInt();
    Serial.printf("[Blynk-CMD] V6 Auto = %d (%s)\n",
        state.autoMode, state.autoMode ? "خودکار" : "دستی");
}
