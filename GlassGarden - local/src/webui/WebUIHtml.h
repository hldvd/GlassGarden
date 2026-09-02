#pragma once

/*
------------------------------------------------------------
GlassGarden — Web UI HTML Template

فایل HTML داشبورد Web UI (بدون CSS)
CSS در فایل جداگانه WebUICss.h تعریف شده
و در زمان اجرا جایگذاری می‌شود.

Version : 1.0.0
------------------------------------------------------------
*/

const char WEBUI_HTML[] PROGMEM = R"htmlraw(
<!DOCTYPE html>
<html lang="fa" dir="rtl">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>GlassGarden — داشبورد</title>
<style>{{CSS}}</style>
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
let ws = null;
let reconnectTimer = null;
let lastState = {};

function connect(){
  if (ws) return;
  
  ws = new WebSocket('ws://'+location.host+'/ws');
  
  ws.onopen = ()=>{
    clearTimeout(reconnectTimer);
    reconnectTimer = null;
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
    ws = null;
    if (!reconnectTimer) {
      reconnectTimer = setTimeout(()=>{
        reconnectTimer = null;
        connect();
      }, 5000);
    }
  };
  
  ws.onerror = ()=>{
    ws.close();
  };
}

window.onbeforeunload = function(){
  clearTimeout(reconnectTimer);
  reconnectTimer = null;
  if(ws) { ws.close(); ws = null; }
};

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
)htmlraw";