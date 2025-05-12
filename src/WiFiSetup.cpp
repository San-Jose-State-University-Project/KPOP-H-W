#include "WiFiSetup.h"
#include "I2C.h"

void WiFiSetup::begin(const char* apName) {
    wifiManager.setDebugOutput(false);
    wifiManager.setConfigPortalTimeout(5);
    bool res = wifiManager.autoConnect(apName,"password");
    if (res) printLCD("Connect","WiFi");
    else printLCD("Disconnect","WiFi");
}