#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
// LCD 객체 생성 - 주소 0x27, 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Wire.begin(D2, D1); // NodeMCU 전용: SDA = D2 (GPIO4), SCL = D1 (GPIO5)
    lcd.init();         // LCD 초기화
    lcd.backlight();    // 백라이트 켜기

    lcd.setCursor(0, 0);
    lcd.print("NodeMCU Hello!");
    lcd.setCursor(0, 1);
    lcd.print("LCD 0x27 OK");
}

void loop() {
    // loop 비워둬도 OK
}