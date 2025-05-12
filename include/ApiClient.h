#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>

class ApiClient{
private:
  char* apiUrl;
public:
  void setApiUrl(char* url);
  void httpGet(const char *endPoint = "/");
};

#endif