/*
 * 作业2: 使用 millis() 控制 LED 以 1Hz 频率稳定闪烁
 * 功能: 非阻塞方式控制 LED 闪烁（500ms 亮 + 500ms 灭 = 1Hz）
 */

const int ledPin = 2;
bool ledState = false;
unsigned long previousMillis = 0;
const long interval = 500;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    Serial.println(ledState ? "LED ON" : "LED OFF");
  }
}
