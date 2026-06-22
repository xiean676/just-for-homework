# 作业2：使用 millis() 控制 LED 1Hz 闪烁

## 实验目标
使用 `millis()` 函数替代 `delay()` 实现非阻塞方式控制 LED 以 1Hz 频率稳定闪烁。

## 技术要点
- `millis()` 返回系统运行毫秒数，用于非阻塞计时
- 通过比较当前时间与上次时间差值控制闪烁间隔
- 闪烁频率：500ms 亮 + 500ms 灭 = 1Hz

## 代码说明
```cpp
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
}
```

## 运行效果
LED 以 1Hz 频率稳定闪烁，串口输出 "LED ON" / "LED OFF" 状态。
