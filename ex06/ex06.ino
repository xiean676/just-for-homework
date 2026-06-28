/*
 * 作业6: 警车双闪灯效（双通道PWM）
 * 功能: 两个LED呈现反相关系的渐变闪烁效果
 * - 当LED1亮度从0增加到255时，LED2亮度从255减小到0
 * - 一个变亮的同时另一个逐渐变暗，过渡柔和
 * - 周期约2秒
 */

const int ledPin1 = 2;    // 第一个LED GPIO2
const int ledPin2 = 15;   // 第二个LED GPIO15
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin1, freq, resolution);
  ledcAttach(ledPin2, freq, resolution);
  Serial.println("警车双闪灯效已启动");
}

void loop() {
  unsigned long t = millis() % 4000; // 周期4000ms

  int brightness;
  if (t < 2000) {
    // 前2秒: LED1从暗到亮，LED2从亮到暗
    brightness = map(t, 0, 2000, 0, 255);
  } else {
    // 后2秒: LED1从亮到暗，LED2从暗到亮
    brightness = map(t, 2000, 4000, 255, 0);
  }

  ledcWrite(ledPin1, brightness);
  ledcWrite(ledPin2, 255 - brightness);

  delay(10);
}
