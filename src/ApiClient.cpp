#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "ApiClient.h"

#define URL_ADDR 0
#define ENDPOINT_ADDR 128
#define EEPROM_SIZE 512  // 여기만 정의하고 헤더에서는 제거하세요

ApiClient::ApiClient(PageManager *pm) {
    apiUrl = "";
    endPoint = "";
    EEPROM.begin(EEPROM_SIZE);
    pageManager = pm;
}

void ApiClient::setApiUrl(const String &url) {
    apiUrl = url;
    saveToEEPROM();
}

void ApiClient::setEndPoint(const String &point) {
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
            Serial.println("[HTTP] Payload:");
            Serial.println(payload);

            Serial.print("[MEMORY] Free Heap: ");
            Serial.println(ESP.getFreeHeap());

            StaticJsonDocument<2048> doc;  // 또는 필요시 DynamicJsonDocument  // 최신 ArduinoJson에서는 JsonDocument 권장
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                if (doc.is<JsonArray>()) {
                    Serial.print("[API] 페이지 수: ");
                    Serial.println(pageManager->getMaxPage());
                    int maxAddCount = 4 - pageManager->getMaxPage();
                    Serial.println(maxAddCount);
                    int arrSize = static_cast<int>(doc.size());
                    int pageCount = (maxAddCount < arrSize) ? maxAddCount : arrSize;

                    for (int i = 0; i < maxAddCount; i++) {
                        pageManager->addPage(doc[i]["title"].as<String>(), doc[i]["videoId"].as<String>());
                    }

                    Serial.println("[API] 제목 예시: " + doc[0]["title"].as<String>());
                } else {
                    Serial.println("[API] JSON은 배열이 아님 또는 크기 부족");
                }
            } else {
                Serial.print("[JSON ERROR] ");
                Serial.println(error.c_str());
            }
        } else {
            Serial.print("[HTTP ERROR] Code: ");
            Serial.println(httpCode);
            Serial.println(http.getString());
        }

        http.end();
    } else {
        Serial.println("[HTTP] 연결 실패");
    }
}

void ApiClient::saveToEEPROM() {
    for (size_t i = 0; i < apiUrl.length(); i++) {
        EEPROM.write(URL_ADDR + i, apiUrl[i]);
    }
    EEPROM.write(URL_ADDR + apiUrl.length(), '\0');

    for (size_t i = 0; i < endPoint.length(); i++) {
        EEPROM.write(ENDPOINT_ADDR + i, endPoint[i]);
    }
    EEPROM.write(ENDPOINT_ADDR + endPoint.length(), '\0');

    EEPROM.commit();
    Serial.println("[EEPROM] 저장 완료");
}

void ApiClient::loadFromEEPROM() {
    char urlBuf[128] = {0};
    char epBuf[128] = {0};

    for (int i = 0; i < 128; i++) {
        char c = EEPROM.read(URL_ADDR + i);
        urlBuf[i] = c;
        if (c == '\0') break;
    }

    for (int i = 0; i < 128; i++) {
        char c = EEPROM.read(ENDPOINT_ADDR + i);
        epBuf[i] = c;
        if (c == '\0') break;
    }

    apiUrl = String(urlBuf);
    endPoint = String(epBuf);

    Serial.println("[EEPROM] 불러온 URL: " + apiUrl);
    Serial.println("[EEPROM] 불러온 EP : " + endPoint);
}