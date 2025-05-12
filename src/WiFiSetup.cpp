#include "WiFiSetup.h"
#include "I2C.h"

void WiFiSetup::begin(const char* apName) {
    WiFiManager wifiManager;
    if (wifiManager.autoConnect()) printLCD("Connect","WiFi");
    else printLCD("Disconnect","WiFi");
    wifiManager.startConfigPortal("ESP8266","password");
}

String WiFiSetup::getLocalIP() {
    return WiFi.localIP().toString();
}