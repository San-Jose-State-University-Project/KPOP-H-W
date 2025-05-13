#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "ApiClient.h"

ApiClient::ApiClient() {
    apiUrl = "";
    endPoint = "";
    EEPROM.begin(EEPROM_SIZE);
}
void ApiClient::setApiUrl(char* url) {
    apiUrl = url;
    saveToEEPROM();
}

void ApiClient::setEndPoint(char* point) {
    endPoint = point;
    saveToEEPROM();
}

void ApiClient::httpGet() {
    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(10000);
    HTTPClient http;

    String fullUrl = apiUrl + endPoint;
    Serial.println("[HTTP GET] " + fullUrl);

    if (http.begin(client, fullUrl)) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error && doc.is<JsonArray>() && doc.size() > 0) {
                String title = doc[0].as<String>();
                Serial.println("[API] : " + title);
            } else {
                Serial.println("[API] JSON 파싱 실패 또는 빈 배열");
            }
        } else {
            Serial.println("[HTTP ERROR] " + String(httpCode));
            Serial.println(http.getString());
        }
        http.end();
    } else {
        Serial.println("[API] 연결 실패");
    }
}

void ApiClient::saveToEEPROM() {
    // URL 저장
    for (int i = 0; i < apiUrl.length(); i++) {
        EEPROM.write(URL_ADDR + i, apiUrl[i]);
    }
    EEPROM.write(URL_ADDR + apiUrl.length(), '\0');

    // EndPoint 저장
    for (int i = 0; i < endPoint.length(); i++) {
        EEPROM.write(ENDPOINT_ADDR + i, endPoint[i]);
    }
    EEPROM.write(ENDPOINT_ADDR + endPoint.length(), '\0');

    EEPROM.commit();
    Serial.println("[EEPROM] 저장 완료");
}

void ApiClient::loadFromEEPROM() {
    char urlBuf[128];
    char epBuf[128];

    for (int i = 0; i < 128; i++) {
        char c = EEPROM.read(URL_ADDR + i);
        if (c == '\0') {
            urlBuf[i] = '\0';
            break;
        }
        urlBuf[i] = c;
    }

    for (int i = 0; i < 128; i++) {
        char c = EEPROM.read(ENDPOINT_ADDR + i);
        if (c == '\0') {
            epBuf[i] = '\0';
            break;
        }
        epBuf[i] = c;
    }

    apiUrl = String(urlBuf);
    endPoint = String(epBuf);
    Serial.println("[EEPROM] 불러온 URL: " + apiUrl);
    Serial.println("[EEPROM] 불러온 EP : " + endPoint);
}