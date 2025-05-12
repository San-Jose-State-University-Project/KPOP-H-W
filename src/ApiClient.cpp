#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "ApiClient.h"

void ApiClient::setApiUrl(char* url) {
    apiUrl = url;
}
void ApiClient::httpGet(const char *endPoint) {
    WiFiClientSecure client;
    client.setInsecure();  // 인증서 검증 안 함
    client.setTimeout(10000);
    HTTPClient http;
    Serial.println(String(apiUrl)+String(endPoint));
    if (http.begin(client, String(apiUrl)+String(endPoint))) {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();

            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);

            if (!error && doc.is<JsonArray>() && doc.size() > 0) {
                String title = doc[0]["title"].as<String>();
                Serial.println("[API] Title: "+title);
            } else {
                Serial.println("[API] JSON 파싱 실패 또는 빈 배열");
            }
        } else {
            Serial.println(http.getString());
        }
        http.end();
    } else {
        Serial.println("[API] 연결 실패");
    }
}