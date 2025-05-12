#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

void initLCD();
void printLCD(const String &line1, const String &line2);

#endif