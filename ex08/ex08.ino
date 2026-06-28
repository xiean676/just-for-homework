/*
 * 作业8: 物联网安防报警器模拟实验
 * 功能: ESP32作为安防主机，网页端控制布防/撤防，触摸触发报警
 * - 网页提供"布防"和"撤防"按钮
 * - 布防状态下触摸引脚触发LED高频闪烁报警
 * - 只有通过网页点击"撤防"才能停止报警
 */

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const int ledPin = 2;
const int touchPin = 4;

WebServer server(80);

bool armed = false;       // 布防状态
bool alarmActive = false; // 报警状态
bool ledOn = false;       // LED闪烁状态
unsigned long lastBlinkTime = 0;

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 安防报警器</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; background: #1a1a2e; color: #eee; }
    h1 { color: #e94560; }
    .status { font-size: 24px; margin: 20px 0; padding: 15px; border-radius: 10px; }
    .disarmed { background: #0f3460; color: #4ecca3; }
    .armed { background: #533483; color: #e94560; }
    .alarm { background: #e94560; color: #fff; animation: blink 0.3s infinite; }
    @keyframes blink { 50% { opacity: 0.5; } }
    button { font-size: 20px; padding: 15px 40px; margin: 10px; border: none; border-radius: 10px; cursor: pointer; }
    .arm-btn { background: #e94560; color: white; }
    .disarm-btn { background: #4ecca3; color: #1a1a2e; }
    button:hover { opacity: 0.8; }
  </style>
</head>
<body>
  <h1>ESP32 安防报警器</h1>
  <div id="status" class="status disarmed">未布防</div>
  <button class="arm-btn" onclick="send('/arm')">布防</button>
  <button class="disarm-btn" onclick="send('/disarm')">撤防</button>
  <script>
    function send(url) {
      fetch(url).then(r => r.text()).then(t => {
        document.getElementById('status').innerText = t;
        document.getElementById('status').className = 'status ' + (t.includes('布防') ? (t.includes('报警') ? 'alarm' : 'armed') : 'disarmed');
      });
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleArm() {
  armed = true;
  alarmActive = false;
  Serial.println("系统已布防");
  server.send(200, "text/plain", "已布防 - 等待触发");
}

void handleDisarm() {
  armed = false;
  alarmActive = false;
  ledOn = false;
  digitalWrite(ledPin, LOW);
  Serial.println("系统已撤防");
  server.send(200, "text/plain", "未布防");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("安防系统已启动");
}

void loop() {
  server.handleClient();

  // 布防状态下检测触摸
  if (armed && !alarmActive) {
    int touchValue = touchRead(touchPin);
    if (touchValue < 600) {
      alarmActive = true;
      lastBlinkTime = millis();
      Serial.println("报警触发!");
    }
  }

  // 报警状态下LED高频闪烁
  if (alarmActive) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= 200) { // 200ms闪烁间隔
      ledOn = !ledOn;
      digitalWrite(ledPin, ledOn ? HIGH : LOW);
      lastBlinkTime = now;
    }
  }
}
