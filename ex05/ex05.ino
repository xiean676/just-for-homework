/*
 * 作业5: 多档位触摸调速呼吸灯
 * 功能: LED持续呼吸灯效果，触摸切换3个速度档位
 * - 档位1: 缓慢呼吸 (delay=10ms, 步长=1)
 * - 档位2: 中速呼吸 (delay=5ms, 步长=2)
 * - 档位3: 急促呼吸 (delay=2ms, 步长=5)
 * - 每次触摸循环切换档位 1→2→3→1
 */

const int touchPin = 4;    // 触摸引脚 GPIO4
const int ledPin = 2;      // 板载LED GPIO2
const int freq = 5000;
const int resolution = 8;

int speedLevel = 1;           // 当前速度档位 (1/2/3)
bool lastTouchState = false;  // 上一次触摸状态
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, freq, resolution);
  Serial.println("触摸调速呼吸灯已启动，当前档位: 1");
}

void loop() {
  // 检测触摸切换档位
  bool currentTouch = (touchRead(touchPin) < 30);
  if (currentTouch && !lastTouchState) {
    unsigned long now = millis();
    if (now - lastDebounceTime > debounceDelay) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1;
      lastDebounceTime = now;
      Serial.print("档位切换: ");
      Serial.println(speedLevel);
    }
  }
  lastTouchState = currentTouch;

  // 根据档位设置呼吸灯参数
  int step;
  int delayMs;
  switch (speedLevel) {
    case 1: step = 1;  delayMs = 10; break; // 缓慢
    case 2: step = 2;  delayMs = 5;  break; // 中速
    case 3: step = 5;  delayMs = 2;  break; // 急促
    default: step = 1; delayMs = 10; break;
  }

  // 呼吸灯：从暗到亮
  for (int duty = 0; duty <= 255; duty += step) {
    ledcWrite(ledPin, duty);
    delay(delayMs);
    // 呼吸过程中也要检测触摸
    bool touch = (touchRead(touchPin) < 30);
    if (touch && !lastTouchState) {
      unsigned long now = millis();
      if (now - lastDebounceTime > debounceDelay) {
        speedLevel++;
        if (speedLevel > 3) speedLevel = 1;
        lastDebounceTime = now;
        Serial.print("档位切换: ");
        Serial.println(speedLevel);
        switch (speedLevel) {
          case 1: step = 1;  delayMs = 10; break;
          case 2: step = 2;  delayMs = 5;  break;
          case 3: step = 5;  delayMs = 2;  break;
        }
      }
    }
    lastTouchState = touch;
  }

  // 呼吸灯：从亮到暗
  for (int duty = 255; duty >= 0; duty -= step) {
    ledcWrite(ledPin, duty);
    delay(delayMs);
    // 呼吸过程中也要检测触摸
    bool touch = (touchRead(touchPin) < 30);
    if (touch && !lastTouchState) {
      unsigned long now = millis();
      if (now - lastDebounceTime > debounceDelay) {
        speedLevel++;
        if (speedLevel > 3) speedLevel = 1;
        lastDebounceTime = now;
        Serial.print("档位切换: ");
        Serial.println(speedLevel);
        switch (speedLevel) {
          case 1: step = 1;  delayMs = 10; break;
          case 2: step = 2;  delayMs = 5;  break;
          case 3: step = 5;  delayMs = 2;  break;
        }
      }
    }
    lastTouchState = touch;
  }
}
