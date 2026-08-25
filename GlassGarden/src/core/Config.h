#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//======================================================
// GlassGarden Configuration
// نسخه : 1.0.0
//======================================================

//-------------- Project -----------------

#define PROJECT_NAME       "GlassGarden"
#define PROJECT_VERSION    "1.0.0"

//-------------- Serial ------------------

constexpr uint32_t SERIAL_BAUDRATE = 115200;

//-------------- WiFi --------------------

constexpr char WIFI_SSID[] = "Xiaomi 11 Lite 5G NE";
constexpr char WIFI_PASSWORD[] = "sma28152";

//-------------- Blynk -------------------

#define BLYNK_TEMPLATE_ID   "TMPL6BugEIoP9"
#define BLYNK_TEMPLATE_NAME "GlassGarden v1"
#define BLYNK_AUTH_TOKEN    "eCG1Cwp3-E-DE_tUG3WYD-UlF5bseUZp"

//-------------- GPIO --------------------

constexpr uint8_t LIGHT_PIN  = 4;
constexpr uint8_t FOGGER_PIN = 32;
constexpr uint8_t FAN_PIN    = 22;
constexpr uint8_t PUMP_PIN   = 18;

//-------------- Sensor ------------------

constexpr uint8_t DHT_PIN = 27;

// نوع سنسور : 11 = DHT11 | 22 = DHT22/AM2302 | 21 = DHT21
constexpr uint8_t DHT_TYPE = 22;

//-------------- Relay Logic -------------

constexpr bool OUTPUT_ACTIVE_HIGH = true;

//-------------- Automation --------------

constexpr bool AUTOMATION_ENABLED = true;

// فن بر اساس دما (سانتی‌گراد)
constexpr float FAN_TEMP_ON  = 28.0f;
constexpr float FAN_TEMP_OFF = 26.0f;

// فن بر اساس رطوبت (درصد) - رطوبت بالا هم فن را روشن می‌کند
constexpr float FAN_HUMIDITY_ON  = 75.0f;
constexpr float FAN_HUMIDITY_OFF = 70.0f;

// مه‌ساز بر اساس رطوبت (درصد)
constexpr float FOGGER_HUMIDITY_ON  = 60.0f;
constexpr float FOGGER_HUMIDITY_OFF = 70.0f;

// روشنایی بر اساس زمان‌بندی (ساعت 24 ساعته)
constexpr int LIGHT_ON_HOUR  = 8;
constexpr int LIGHT_OFF_HOUR = 20;

// فن بر اساس زمان‌بندی (علاوه بر دما)
constexpr int FAN_ON_HOUR  = 8;
constexpr int FAN_OFF_HOUR = 20;

// مه‌ساز بر اساس زمان‌بندی (علاوه بر رطوبت)
constexpr int FOGGER_ON_HOUR  = 6;
constexpr int FOGGER_OFF_HOUR = 22;

// پمپ بر اساس زمان‌بندی (پمپ آبشار - هم‌زمان با ساعات روشنایی)
constexpr int PUMP_ON_HOUR  = 8;
constexpr int PUMP_OFF_HOUR = 20;

//-------------- NTP (Time Sync) ---------

constexpr char NTP_SERVER[] = "pool.ntp.org";

// آفست منطقهٔ زمانی به ثانیه (پیش‌فرض : ایران +3:30)
constexpr long NTP_GMT_OFFSET_SEC = 12600;
constexpr int  NTP_DAYLIGHT_OFFSET_SEC = 0;

#endif