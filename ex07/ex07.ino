/*
 * 作业7: Web网页端无极调光器
 * 功能: 手机或电脑浏览器打开ESP32网页，拖动滑动条控制LED亮度
 * - 网页显示滑动条 (range 0-255)
 * - JavaScript实时发送滑动条数值到ESP32
 * - ESP32解析数值并控制PWM输出
 */

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_SSID";       // WiFi名称，烧录前修改
const char* password = "YOUR_PASSWORD"; // WiFi密码，烧录前修改

const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

WebServer server(80);

// 嵌入式HTML页面
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 无极调光器</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; background: #1a1a2e; color: #eee; }
    h1 { color: #e94560; }
    .slider-container { margin: 30px auto; width: 80%; }
    input[type=range] { width: 100%; height: 20px; -webkit-appearance: none; background: #16213e; border-radius: 10px; outline: none; }
    input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; width: 30px; height: 30px; background: #e94560; border-radius: 50%; cursor: pointer; }
    #value { font-size: 48px; color: #e94560; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>ESP32 无极调光器</h1>
  <div class="slider-container">
    <input type="range" id="slider" min="0" max="255" value="0" oninput="sendValue(this.value)">
  </div>
  <div id="value">0</div>
  <script>
    function sendValue(val) {
      document.getElementById('value').innerText = val;
      fetch('/set?val=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSet() {
  if (server.hasArg("val")) {
    int val = server.arg("val").toInt();
    val = constrain(val, 0, 255);
    ledcWrite(ledPin, val);
    Serial.print("亮度设置: ");
    Serial.println(val);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing val");
  }
}

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);

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
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
}
