#include "LedManager.h"

LedManager::LedManager(uint8_t r, uint8_t g, uint8_t b)
  : redPin(r), greenPin(g), bluePin(b) {

  emotionMap["angry"]    = {255, 0, 0};     // 분노: 빨강
  emotionMap["disgust"]  = {0, 255, 0};     // 역겨움: 초록
  emotionMap["fear"]     = {128, 0, 128};   // 두려움: 보라
  emotionMap["happy"]    = {255, 255, 0};   // 기쁨: 노랑
  emotionMap["neutral"]  = {0, 0, 0};       // 중립: 꺼짐
  emotionMap["sad"]      = {0, 0, 255};     // 슬픔: 파랑
  emotionMap["surprise"] = {255, 255, 255}; // 놀람: 흰색
}

void LedManager::begin() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void LedManager::applyColor(const RgbColor &color) {
  analogWrite(redPin, color.r);
  analogWrite(greenPin, color.g);
  analogWrite(bluePin, color.b);
}

void LedManager::setEmotion(const String &emotion) {
  if (emotionMap.count(emotion)) {
    applyColor(emotionMap[emotion]);
  } else {
    Serial.println("Unknown emotion: " + emotion);
  }
}

void LedManager::setCustomEmotion(const String &emotion, const RgbColor &color) {
  emotionMap[emotion] = color;
}