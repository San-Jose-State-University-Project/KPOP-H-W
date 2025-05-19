#include <Arduino.h>
#include "WiFiSetup.h"
#include "I2C.h"
#include "ApiClient.h"
#include "LedManager.h"
#include "PageManager.h"
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
    Serial.println(ESP.getCycleCount());
    webClient.handleClient();
    apiClient.httpGet();
}


/*
*#include "PageManager.h"

PageManager pageManager;

void IRAM_ATTR handleButtonPress() {
    pageManager.nextPage();
}

void setup() {
    pinMode(D3, INPUT_PULLUP); // 버튼 연결된 핀
    attachInterrupt(digitalPinToInterrupt(D3), handleButtonPress, FALLING);

    pageManager.addPage("Page 1", "WiFi Info");
    pageManager.addPage("Page 2", "API Title");
    pageManager.addPage("Page 3", "Other Info");

    pageManager.showCurrentPage();
}
 */