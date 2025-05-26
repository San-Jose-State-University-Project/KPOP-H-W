#include "LedManager.h"

LedManager::LedManager(uint8_t r, uint8_t g, uint8_t b)
  : redPin(r), greenPin(g), bluePin(b) {

  emotionMap["joy"]    = {0, 0, 255};
  emotionMap["sadness"]  = {255, 255, 0};
  emotionMap["surprise"]     = {0, 90, 255};
  emotionMap["anger"]    = {0, 255, 255};
  emotionMap["neutral"]  = {0, 0, 0};
  emotionMap["disgust"]      = {255, 0, 255};
  emotionMap["fear"] = {127, 255, 127};
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