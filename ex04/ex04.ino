/*
 * 作业4: 触摸自锁开关
 * 功能: 触摸一下LED亮起并保持，再触摸一下LED熄灭
 * - 使用触摸引脚检测手指触碰
 * - 边缘检测：仅在"未触摸→触摸"瞬间触发
 * - 软件防抖：50ms消抖防止手抖误触发
 */

const int touchPin = 4;    // 触摸引脚 GPIO4 (T0通道)
const int ledPin = 2;      // 板载LED GPIO2

bool ledState = false;       // LED当前状态
bool lastTouchState = false; // 上一次的触摸状态
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 防抖延时50ms

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.println("触摸自锁开关已启动");
}

void loop() {
  int touchValue = touchRead(touchPin);
  bool currentTouch = (touchValue < 30); // 小于30视为触摸

  // 边缘检测：从未触摸变为触摸的瞬间
  if (currentTouch && !lastTouchState) {
    unsigned long now = millis();
    // 软件防抖
    if (now - lastDebounceTime > debounceDelay) {
      ledState = !ledState; // 翻转LED状态
      digitalWrite(ledPin, ledState ? HIGH : LOW);
      lastDebounceTime = now;
      Serial.print("触摸检测! LED状态: ");
      Serial.println(ledState ? "亮" : "灭");
    }
  }

  lastTouchState = currentTouch;
  delay(10);
}
