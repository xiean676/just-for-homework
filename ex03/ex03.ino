/*
 * 作业3: 使用 millis() 控制 LED 产生 SOS 闪烁信息
 * SOS 信号: 三短闪 + 三长闪 + 三短闪，播放完毕后停顿 2 秒
 * 短闪: 200ms 亮 + 200ms 灭
 * 长闪: 600ms 亮 + 200ms 灭
 */

const int ledPin = 2;
bool ledState = false;
unsigned long previousMillis = 0;

// SOS 时序表: [亮灯时长, 熄灯时长]
// 三短 + 间隔 + 三长 + 间隔 + 三短 + 长停顿
const unsigned long sosPattern[][2] = {
  // S: 三短闪
  {200, 200}, {200, 200}, {200, 200},
  {200, 200},
  // O: 三长闪
  {600, 200}, {600, 200}, {600, 200},
  {200, 200},
  // S: 三短闪
  {200, 200}, {200, 200}, {200, 200},
  {200, 2000}
};

const int patternLength = sizeof(sosPattern) / sizeof(sosPattern[0]);
int patternIndex = 0;
bool isOnPhase = true;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("SOS Signal Start");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= (isOnPhase ? sosPattern[patternIndex][0] : sosPattern[patternIndex][1])) {
    previousMillis = currentMillis;

    if (isOnPhase) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
      isOnPhase = false;
    } else {
      isOnPhase = true;
      patternIndex++;
      if (patternIndex >= patternLength) {
        patternIndex = 0;
        Serial.println("SOS cycle completed");
      }
    }
  }
}
