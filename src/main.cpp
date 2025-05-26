#include <Arduino.h>
#include "WiFiSetup.h"
#include <I2C.h>
#include <ApiClient.h>
#include <LedManager.h>
#include <PageManager.h>
#include "WebClient.h"

#define BUTTON1_PIN D3  // GPIO0
#define BUTTON2_PIN D4  // GPIO2
#define DEBOUNCE_CYCLES 1600000  // 약 200ms (80MHz 기준)

volatile uint32_t lastButton1Cycle = 0;
volatile uint32_t lastButton2Cycle = 0;

WiFiSetup wifi;
LedManager ledManager(D0, D6, D7);
PageManager pageManager(&ledManager);
ApiClient apiClient(&pageManager);
WebClient webClient(&apiClient);

unsigned long bothPressedStart = 0;
const unsigned long RESET_HOLD_TIME = 5000; // 5초

void IRAM_ATTR handleButton1() {
    uint32_t now = ESP.getCycleCount();
    if (now - lastButton1Cycle > DEBOUNCE_CYCLES) {
        lastButton1Cycle = now;
        pageManager.beforePage();
    }
}

void IRAM_ATTR handleButton2() {
    uint32_t now = ESP.getCycleCount();
    if (now - lastButton2Cycle > DEBOUNCE_CYCLES) {
        lastButton2Cycle = now;
        pageManager.nextPage();
    }
}

void setup() {
    Serial.begin(115200);
    initLCD();
    wifi.begin();
    apiClient.setDebug(true);
    apiClient.loadFromEEPROM();
    webClient.setPageManager(&pageManager);
    webClient.begin();
    ledManager.begin();
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2, FALLING);
}

void loop() {
    webClient.handleClient();
    apiClient.httpGet();

    // 두 버튼이 모두 눌렸는지 확인
    bool b1 = digitalRead(BUTTON1_PIN) == LOW;
    bool b2 = digitalRead(BUTTON2_PIN) == LOW;

    if (b1 && b2) {
        if (bothPressedStart == 0) {
            bothPressedStart = millis(); // 눌린 시간 기록 시작
        } else if (millis() - bothPressedStart >= RESET_HOLD_TIME) {
            Serial.println("Wi-Fi 설정 리셋!");
            wifi.resetWiFi();
            delay(1000);
            ESP.restart(); // 리셋 후 재부팅
        }
    } else {
        bothPressedStart = 0; // 하나라도 떼면 시간 초기화
    }
    delay(3000);
}