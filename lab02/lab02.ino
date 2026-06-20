/*
 * 实验2: 基础IO实验 - LED闪烁控制
 * 功能: 使用 ESP32 GPIO 输出控制板载 LED 闪烁
 * - LED 亮 1 秒，灭 1 秒
 * - 串口输出 "LED ON" / "LED OFF"
 */

const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  delay(1000);

  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
