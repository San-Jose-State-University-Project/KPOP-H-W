#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
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
            if (Debug) {
                Serial.println("[HTTP] Payload:");
                Serial.println(payload);

                Serial.print("[API] Free Heap: ");
                Serial.println(ESP.getFreeHeap());
            }
            StaticJsonDocument<2048> doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                if (doc.is<JsonArray>()) {
                    size_t maxAddCount = 5 - pageManager->getMaxPage();
                    size_t arrSize = static_cast<size_t>(doc["emotion_list"].size());
                    size_t pageCount = min(maxAddCount, arrSize);
                    if (Debug) {
                        Serial.print("[API] 페이지: ");
                        Serial.println(pageManager->getCurrentPage());
                    }
                    for (int i = 0; i < pageCount; i++) {
                        pageManager->addPage(doc["emotion_list"][i]["title"].as<String>(), doc[i]["videoId"].as<String>(), doc[i]["emotion"].as<String>());
                    }
                    pageManager->showCurrentPage();
                    Serial.println("[API] 첫 데이터 : " + doc["emotion_list"][0]["title"].as<String>());
                } else {
                    Serial.println("[API] JSON은 배열이 아님 또는 크기 부족");
                }
            } else {
                if (Debug) {
                    Serial.print("[JSON ERROR] ");
                    Serial.println(error.c_str());
                }
            }
        } else {
            if (Debug) {
                Serial.print("[HTTP ERROR] Code: ");
                Serial.println(httpCode);
                Serial.println(http.getString());
            }
        }

        http.end();
    } else {
        if (Debug) Serial.println("[HTTP] 연결 실패");
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
    if (Debug) Serial.println("[EEPROM] 저장 완료");
    pageManager->addPage(WiFi.localIP().toString(),endPoint);
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

    if (Debug) {
        Serial.println("[EEPROM] 불러온 URL: " + apiUrl);
        Serial.println("[EEPROM] 불러온 EP : " + endPoint);
    }
    pageManager->addPage(WiFi.localIP().toString(),endPoint);
}
void ApiClient::setDebug(bool debug) {
    Debug = debug;
}
