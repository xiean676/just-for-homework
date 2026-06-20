/*
 * 实验3: LED呼吸灯实验
 * 功能: 使用 PWM 技术实现 LED 呼吸灯效果
 * - LED 亮度从暗到亮，再从亮到暗，循环往复
 */

const int ledPin = 2;
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
}

void loop() {
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  Serial.println("Breathing cycle completed");
}
