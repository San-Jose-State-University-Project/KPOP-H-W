#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include <map>

struct RgbColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class LedManager {
private:
  uint8_t redPin, greenPin, bluePin;
  std::map<String, RgbColor> emotionMap;

  void applyColor(const RgbColor &color);

public:
  LedManager(uint8_t r, uint8_t g, uint8_t b);
  void begin();
  void setEmotion(const String &emotion);
  void setCustomEmotion(const String &emotion, const RgbColor &color);
};

#endif