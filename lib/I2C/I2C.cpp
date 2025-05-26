#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initLCD() {
    Wire.begin(D2, D1);
    lcd.init();
    lcd.backlight();
}

void printLCD(const String &line1, const String &line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}