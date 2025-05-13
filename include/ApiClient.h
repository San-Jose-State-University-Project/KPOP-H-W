// include/ApiClient.h
#ifndef APICLIENT_H
#define APICLIENT_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 256
#define URL_ADDR 0
#define ENDPOINT_ADDR 128

class ApiClient {
private:
  String apiUrl;
  String endPoint;

public:
  ApiClient();
  void setApiUrl(char* url);
  void setEndPoint(char* point);
  void httpGet();

  void saveToEEPROM();
  void loadFromEEPROM();
};

#endif