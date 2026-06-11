/*
 * 实验1: Arduino IDE 安装与 ESP32 环境配置
 * 功能: 验证 Arduino IDE + ESP32 环境是否配置成功
 * - 板载 LED 每秒闪烁一次
 * - 串口每秒输出 "Hello ESP32!"
 */

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.println("Hello ESP32!");
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
