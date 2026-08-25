/*
------------------------------------------------------------
GlassGarden

File : BlynkHandlers.cpp

وظیفه:
دریافت فرمان‌های اپلیکیشن Blynk و انتقال آن‌ها
به DeviceManager

Version : 1.0.0
------------------------------------------------------------
*/

#include "../core/Config.h"

#include <Arduino.h>
#include <BlynkApiArduino.h>

#include "../core/Datastreams.h"
#include "../devices/DeviceManager.h"
#include "../state/StateManager.h"
#include "BlynkHandlersHook.h"

//------------------------------------------------------------
// این تابع هیچ کاری انجام نمی‌دهد؛ فقط با فراخوانی آن از main.cpp
// به لینکر می‌گوییم این فایل استفاده می‌شود و نباید حذف شود
// (رفع مشکل عدم‌اجرای BLYNK_WRITE ها؛ نگاه کنید به BlynkHandlers.h)
//------------------------------------------------------------

void registerBlynkHandlers()
{
    // عمداً خالی است
}

//------------------------------------------------------------
// Light
//------------------------------------------------------------

BLYNK_WRITE(V0)
{
    Serial.print("[Blynk-CMD] Light received = ");
    Serial.println(param.asInt());

    if (param.asInt())
        devices.lightOn();
    else
        devices.lightOff();
}

//------------------------------------------------------------
// Fogger
//------------------------------------------------------------

BLYNK_WRITE(V3)
{
    if (param.asInt())
        devices.foggerOn();
    else
        devices.foggerOff();
}

//------------------------------------------------------------
// Fan
//------------------------------------------------------------

BLYNK_WRITE(V4)
{
    if (param.asInt())
        devices.fanOn();
    else
        devices.fanOff();
}

//------------------------------------------------------------
// Pump
//------------------------------------------------------------

BLYNK_WRITE(V5)
{
    if (param.asInt())
        devices.pumpOn();
    else
        devices.pumpOff();
}

//------------------------------------------------------------
// AUTO / MANUAL
//------------------------------------------------------------

BLYNK_WRITE(V6)
{
    state.autoMode = param.asInt();

    Serial.print("[Mode] Auto = ");
    Serial.println(state.autoMode ? "ON (خودکار)" : "OFF (دستی)");
}