#include "WebUIManager.h"
#include "../core/Config.h"
#include "../state/StateManager.h"
#include "../devices/DeviceManager.h"
#include "../automation/AutomationManager.h"
#include "Images.h"

WebUIManager webUI;

// ============================================================
// HTML Template
// ============================================================
// از __LOGO_URL__ و __NATURE_URL__ به عنوان مارکر استفاده می‌شود
// ============================================================

const char WebUIManager::INDEX_HTML_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>GlassGarden — داشبورد</title>
<style>
  *{box-sizing:border-box;margin:0;padding:0}
  body{font-family:'Segoe UI',Tahoma,sans-serif;background:#0f172a;color:#e2e8f0;line-height:1.6;padding:16px}
  .container{max-width:800px;margin:0 auto}

  header {
    background-color: #e3d9ce;
    color: #0f172a;
    border-radius: 20px;
    padding: 20px 40px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    position: relative;
    overflow: hidden;
    min-height: 100px;
  }

  header::before {
    content: "";
    position: absolute;
    left: 30px;
    top: 50%;
    transform: translateY(-50%);
    width: 120px;
    height: 120px;
    background-image: __LOGO_URL__;
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;
    z-index: 0;
  }

  header::after {
    content: "";
    position: absolute;
    right: 30px;
    top: 50%;
    transform: translateY(-50%);
    width: 220px;
    height: 120px;
    background-image: __NATURE_URL__;
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;
    z-index: 0;
  }

  .status {
    position: relative;
    z-index: 1;
    margin: 0 auto;
    display: flex;
    align-items: center;
    gap: 8px;
    background: rgba(255,255,255,0.3);
    padding: 8px 20px;
    border-radius: 20px;
    font-size: .9rem;
  }

  .status-dot{width:10px;height:10px;border-radius:50%;background:#ef4444}
  .status-dot.online{background:#22c55e;box-shadow:0 0 8px #22c55e}

  header h1{color:#4a6741;font-size:1.6rem;position:relative;z-index:1}

  .grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:12px;margin-bottom:20px}
  .card{background:#1e293b;border-radius:12px;padding:16px;text-align:center;border:1px solid #334155;transition:.2s}
  .card:hover{border-color:#4ade80}
  .card-icon{font-size:2rem;margin-bottom:6px}
  .card-value{font-size:1.6rem;font-weight:700;color:#4ade80}
  .card-label{font-size:.85rem;color:#94a3b8;margin-top:4px}

  .controls{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:12px;margin-bottom:20px}
  .ctrl-card{background:#1e293b;border-radius:12px;padding:16px;border:1px solid #334155}
  .ctrl-card h3{font-size:1rem;margin-bottom:12px;color:#cbd5e1;display:flex;align-items:center;gap:8px}
  .switch{position:relative;display:inline-block;width:56px;height:30px}
  .switch input{opacity:0;width:0;height:0}
  .slider{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background:#475569;border-radius:30px;transition:.3s}
  .slider:before{position:absolute;content:"";height:22px;width:22px;left:4px;bottom:4px;background:#fff;border-radius:50%;transition:.3s}
  input:checked + .slider{background:#4ade80}
  input:checked + .slider:before{transform:translateX(26px)}
  .ctrl-row{display:flex;justify-content:space-between;align-items:center;margin-bottom:12px}
  .ctrl-state{font-size:.8rem;color:#94a3b8}

  .mode-bar{background:#1e293b;border-radius:12px;padding:16px;margin-bottom:20px;display:flex;justify-content:space-between;align-items:center;border:1px solid #334155}
  .mode-btn{background:#334155;border:none;color:#e2e8f0;padding:10px 24px;border-radius:8px;cursor:pointer;font-size:.95rem;transition:.2s}
  .mode-btn.active{background:#4ade80;color:#0f172a;font-weight:700}

  .logs{background:#1e293b;border-radius:12px;padding:16px;border:1px solid #334155;max-height:200px;overflow-y:auto}
  .logs h3{margin-bottom:10px;color:#cbd5e1;font-size:1rem}
  .log-entry{font-size:.8rem;padding:4px 0;border-bottom:1px solid #334155;color:#94a3b8}
  .log-entry:last-child{border:none}
  .log-time{color:#64748b}

  .footer{text-align:center;margin-top:24px;font-size:.75rem;color:#64748b}

  @media(max-width:480px){
    header h1{font-size:1.2rem}
    .card-value{font-size:1.3rem}
    header{padding:15px 20px}
    header::before, header::after{width:80px;height:80px}
  }
</style>
</head>
<body>
<div class="container">
  <header>
    <div class="status">
      <span class="status-dot" id="connDot"></span>
      <span id="connText">در حال اتصال...</span>
    </div>
  </header>

  <div class="mode-bar">
    <span>🎛️ حالت کنترل:</span>
    <div>
      <button class="mode-btn" id="btnAuto" onclick="setMode(true)">خودکار</button>
      <button class="mode-btn" id="btnManual" onclick="setMode(false)">دستی</button>
    </div>
  </div>

  <div class="grid">
    <div class="card">
      <div class="card-icon">🌡️</div>
      <div class="card-value" id="valTemp">--</div>
      <div class="card-label">دما (°C)</div>
    </div>
    <div class="card">
      <div class="card-icon">💧</div>
      <div class="card-value" id="valHum">--</div>
      <div class="card-label">رطوبت (%)</div>
    </div>
    <div class="card">
      <div class="card-icon">🚰</div>
      <div class="card-value" id="valWater">--</div>
      <div class="card-label">آب مخزن (%)</div>
    </div>
    <div class="card">
      <div class="card-icon">🛡️</div>
      <div class="card-value" id="valSafe">خوب</div>
      <div class="card-label">وضعیت ایمنی</div>
    </div>
  </div>

  <div class="controls">
    <div class="ctrl-card">
      <h3>💡 روشنایی</h3>
      <div class="ctrl-row">
        <span class="ctrl-state" id="stLight">خاموش</span>
        <label class="switch"><input type="checkbox" id="swLight" onchange="toggle('light',this.checked)"><span class="slider"></span></label>
      </div>
    </div>
    <div class="ctrl-card">
      <h3>🌀 فن</h3>
      <div class="ctrl-row">
        <span class="ctrl-state" id="stFan">خاموش</span>
        <label class="switch"><input type="checkbox" id="swFan" onchange="toggle('fan',this.checked)"><span class="slider"></span></label>
      </div>
    </div>
    <div class="ctrl-card">
      <h3>💨 مه‌ساز</h3>
      <div class="ctrl-row">
        <span class="ctrl-state" id="stFogger">خاموش</span>
        <label class="switch"><input type="checkbox" id="swFogger" onchange="toggle('fogger',this.checked)"><span class="slider"></span></label>
      </div>
    </div>
    <div class="ctrl-card">
      <h3>💧 پمپ</h3>
      <div class="ctrl-row">
        <span class="ctrl-state" id="stPump">خاموش</span>
        <label class="switch"><input type="checkbox" id="swPump" onchange="toggle('pump',this.checked)"><span class="slider"></span></label>
      </div>
    </div>
  </div>

  <div class="logs">
    <h3>📋 رویدادهای اخیر</h3>
    <div id="logBox"><div class="log-entry"><span class="log-time">--:--</span> سیستم راه‌اندازی شد</div></div>
  </div>

  <div class="footer">GlassGarden v1.2.0 — Web UI Local</div>
</div>

<script>
let ws;
let reconnectTimer;
let lastState = {};

function connect(){
  ws = new WebSocket('ws://'+location.host+'/ws');
  ws.onopen = ()=>{
    clearInterval(reconnectTimer);
    document.getElementById('connDot').classList.add('online');
    document.getElementById('connText').textContent = 'متصل';
    addLog('اتصال WebSocket برقرار شد');
  };
  ws.onmessage = (e)=>{
    const msg = JSON.parse(e.data);
    if(msg.type === 'state') updateUI(msg.payload);
  };
  ws.onclose = ()=>{
    document.getElementById('connDot').classList.remove('online');
    document.getElementById('connText').textContent = 'قطع — تلاش مجدد...';
    reconnectTimer = setInterval(connect,3000);
  };
  ws.onerror = ()=>{ ws.close(); };
}

function updateUI(s){
  lastState = s;
  document.getElementById('valTemp').textContent = s.temperature.toFixed(1);
  document.getElementById('valHum').textContent = s.humidity.toFixed(1);
  document.getElementById('valWater').textContent = s.waterLevelPercent;
  document.getElementById('valSafe').textContent = s.safeMode ? '⚠️ Safe Mode' : '✅ عادی';
  document.getElementById('valSafe').style.color = s.safeMode ? '#ef4444' : '#4ade80';

  setSwitch('swLight','stLight',s.light);
  setSwitch('swFan','stFan',s.fan);
  setSwitch('swFogger','stFogger',s.fogger);
  setSwitch('swPump','stPump',s.pump);

  document.getElementById('btnAuto').classList.toggle('active', s.autoMode);
  document.getElementById('btnManual').classList.toggle('active', !s.autoMode);
}

function setSwitch(idSw,idSt,on){
  document.getElementById(idSw).checked = on;
  document.getElementById(idSt).textContent = on ? 'روشن' : 'خاموش';
  document.getElementById(idSt).style.color = on ? '#4ade80' : '#94a3b8';
}

function toggle(device, on){
  if(ws && ws.readyState===1){
    ws.send(JSON.stringify({type:'cmd', device:device, value:on}));
  }
}

function setMode(auto){
  if(ws && ws.readyState===1){
    ws.send(JSON.stringify({type:'mode', value:auto}));
  }
}

function addLog(text){
  const box = document.getElementById('logBox');
  const t = new Date().toLocaleTimeString('fa-IR',{hour:'2-digit',minute:'2-digit',second:'2-digit'});
  box.insertAdjacentHTML('afterbegin', '<div class="log-entry"><span class="log-time">'+t+'</span> '+text+'</div>');
  if(box.children.length>20) box.lastChild.remove();
}

connect();
</script>
</body>
</html>
)rawliteral";

// ============================================================
// Implementation
// ============================================================

void WebUIManager::begin()
{
    setupRoutes();

    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, 
                      AwsEventType type, void* arg, uint8_t* data, size_t len) {
        this->onWsEvent(client, type, arg, data, len);
    });

    server.addHandler(&ws);
    server.begin();
    Serial.println("[WebUI] Server started on http://" + WiFi.localIP().toString());
}

// ============================================================
// تولید HTML با جایگذاری data URI تصاویر
// ============================================================
String WebUIManager::getIndexedHtml()
{
    String html = FPSTR(INDEX_HTML_TEMPLATE);

    // جایگزینی مارکرهای واضح با data URI کامل
    // استفاده از __LOGO_URL__ به جای var(--img-logo) برای اطمینان از عدم تداخل
    html.replace("__LOGO_URL__", String("url(\"") + FPSTR(CSS_LOGO_URL) + "\")");
    html.replace("__NATURE_URL__", String("url(\"") + FPSTR(CSS_NATURE_URL) + "\")");

    return html;
}

void WebUIManager::setupRoutes()
{
    // صفحه اصلی با تصاویر
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request){
        String html = this->getIndexedHtml();
        request->send(200, "text/html", html);
    });

    // API کنترل تجهیزات
    server.on("/api/control", HTTP_POST,
        [](AsyncWebServerRequest* request){},
        NULL,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total){
            handleApiControl(request, data, len);
        });

    // API وضعیت فعلی
    server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest* request){
        StaticJsonDocument<512> doc;
        doc["temperature"] = state.temperature;
        doc["humidity"] = state.humidity;
        doc["waterLevelPercent"] = state.waterLevelPercent;
        doc["waterEmpty"] = state.waterEmpty;
        doc["light"] = state.light;
        doc["fan"] = state.fan;
        doc["fogger"] = state.fogger;
        doc["pump"] = state.pump;
        doc["autoMode"] = state.autoMode;
        doc["safeMode"] = state.safeMode;
        doc["wifiConnected"] = state.wifiConnected;
        doc["blynkConnected"] = state.blynkConnected;

        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });
}

void WebUIManager::onWsEvent(AsyncWebSocketClient* client, AwsEventType type,
                             void* arg, uint8_t* data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("[WebUI] Client #%u connected from %s\n", 
                      client->id(), client->remoteIP().toString().c_str());
        broadcastState();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("[WebUI] Client #%u disconnected\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            data[len] = 0;
            String msg = (char*)data;

            StaticJsonDocument<256> doc;
            DeserializationError err = deserializeJson(doc, msg);
            if (err) return;

            const char* msgType = doc["type"];

            if (strcmp(msgType, "cmd") == 0)
            {
                const char* device = doc["device"];
                bool value = doc["value"];

                if (!state.autoMode)
                {
                    if (strcmp(device, "light") == 0)   value ? devices.lightOn() : devices.lightOff();
                    else if (strcmp(device, "fan") == 0)    value ? devices.fanOn() : devices.fanOff();
                    else if (strcmp(device, "fogger") == 0) value ? devices.foggerOn() : devices.foggerOff();
                    else if (strcmp(device, "pump") == 0)   value ? devices.pumpOn() : devices.pumpOff();
                }
                else
                {
                    Serial.printf("[WebUI] Ignored manual cmd in AUTO mode: %s=%d\n", device, value);
                }
                broadcastState();
            }
            else if (strcmp(msgType, "mode") == 0)
            {
                state.autoMode = doc["value"];
                Serial.printf("[WebUI] Mode changed to: %s\n", state.autoMode ? "AUTO" : "MANUAL");
                broadcastState();
            }
        }
    }
}

void WebUIManager::broadcastState()
{
    JsonDocument doc;
    doc["type"] = "state";
    JsonObject pl = doc["payload"].to<JsonObject>();
    pl["temperature"] = state.temperature;
    pl["humidity"] = state.humidity;
    pl["waterLevelPercent"] = state.waterLevelPercent;
    pl["waterEmpty"] = state.waterEmpty;
    pl["light"] = state.light;
    pl["fan"] = state.fan;
    pl["fogger"] = state.fogger;
    pl["pump"] = state.pump;
    pl["autoMode"] = state.autoMode;
    pl["safeMode"] = state.safeMode;

    String payload;
    serializeJson(doc, payload);
    ws.textAll(payload);
}

void WebUIManager::handleApiControl(AsyncWebServerRequest* request, uint8_t* data, size_t len)
{
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, (const char*)data);
    if (err)
    {
        request->send(400, "application/json", "{\"error\":\"invalid json\"}");
        return;
    }

    const char* device = doc["device"];
    bool value = doc["value"];

    if (!state.autoMode)
    {
        if (strcmp(device, "light") == 0)   value ? devices.lightOn() : devices.lightOff();
        else if (strcmp(device, "fan") == 0)    value ? devices.fanOn() : devices.fanOff();
        else if (strcmp(device, "fogger") == 0) value ? devices.foggerOn() : devices.foggerOff();
        else if (strcmp(device, "pump") == 0)   value ? devices.pumpOn() : devices.pumpOff();
    }

    broadcastState();
    request->send(200, "application/json", "{\"ok\":true}");
}

void WebUIManager::update()
{
    ws.cleanupClients();

    static unsigned long lastBroadcast = 0;
    if (millis() - lastBroadcast >= 2000)
    {
        lastBroadcast = millis();
        if (ws.count() > 0)
        {
            broadcastState();
        }
    }
}