#ifndef APICLIENT_H
#define APICLIENT_H

#include <Arduino.h>
#include <EEPROM.h>
#include "PageManager.h"

class ApiClient {
public:
  ApiClient(PageManager *pm);
  void setApiUrl(const String &url);
  void setEndPoint(const String &point);
  void httpGet();

  void saveToEEPROM();
  void loadFromEEPROM();
private:
  String apiUrl;
  String endPoint;
  PageManager *pageManager;
};

#endif