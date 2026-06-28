/*
 * 作业9: 实时传感器Web仪表盘
 * 功能: 网页实时显示触摸传感器的模拟量数值
 * - 手靠近引脚时数值变小，离开时数值恢复
 * - 使用AJAX技术每500ms轮询数据
 * - 实现类似仪器仪表的实时监控面板
 */

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int touchPin = 4;

WebServer server(80);

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 传感器仪表盘</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; background: #1a1a2e; color: #eee; }
    h1 { color: #e94560; }
    .dashboard { margin: 30px auto; padding: 40px; background: #16213e; border-radius: 20px; display: inline-block; min-width: 300px; }
    .value { font-size: 72px; color: #4ecca3; font-family: 'Courier New', monospace; }
    .label { font-size: 18px; color: #aaa; margin-top: 10px; }
    .bar-container { width: 80%; height: 20px; background: #0f3460; border-radius: 10px; margin: 20px auto; overflow: hidden; }
    .bar { height: 100%; background: #4ecca3; transition: width 0.3s; border-radius: 10px; }
    .info { font-size: 14px; color: #666; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>ESP32 传感器仪表盘</h1>
  <div class="dashboard">
    <div class="value" id="value">---</div>
    <div class="label">触摸传感器数值 (touchRead)</div>
    <div class="bar-container">
      <div class="bar" id="bar" style="width: 0%"></div>
    </div>
    <div class="info" id="info">手靠近引脚 → 数值变小 | 手离开 → 数值恢复</div>
  </div>
  <script>
    function update() {
      fetch('/data').then(r => r.text()).then(v => {
        document.getElementById('value').innerText = v;
        var pct = Math.min(100, Math.max(0, (1 - v / 100) * 100));
        document.getElementById('bar').style.width = pct + '%';
      });
    }
    setInterval(update, 500);
    update();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  int value = touchRead(touchPin);
  server.send(200, "text/plain", String(value));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP地址: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("仪表盘已启动");
}

void loop() {
  server.handleClient();

  // 串口输出传感器值
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 1000) {
    Serial.print("触摸值: ");
    Serial.println(touchRead(touchPin));
    lastPrint = millis();
  }
}
