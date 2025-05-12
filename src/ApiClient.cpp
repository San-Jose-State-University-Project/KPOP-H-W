#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "ApiClient.h"

const char* apiUrl = "";

void fetchFirstResult() {
    WiFiClientSecure client;
    client.setInsecure();  // 인증서 검증 안 함

    HTTPClient http;
    if (http.begin(client, apiUrl)) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();

            DynamicJsonDocument doc(8192);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error && doc.is<JsonArray>() && doc.size() > 0) {
                String title = doc[0]["title"].as<String>();
                Serial.println("[API] Title: " + title);
            } else {
                Serial.println("[API] JSON 파싱 실패 또는 빈 배열");
            }
        } else {
            Serial.printf("[API] HTTP 오류: %d\n", httpCode);
        }
        http.end();
    } else {
        Serial.println("[API] 연결 실패");
    }
}