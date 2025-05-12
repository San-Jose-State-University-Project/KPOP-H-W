#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

class WiFiSetup {
private:
    WiFiManager wifiManager;
public:
    void begin(const char* apName = "ESP-Setup");
};

#endif