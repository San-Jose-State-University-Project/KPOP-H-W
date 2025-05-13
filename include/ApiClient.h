#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>

class ApiClient{
private:
  const char* apiUrl;
  const char* endPoint;
public:
  ApiClient(const char* url, const char* point);
  void setApiUrl(char* url);
  void setEndPoint(char* point);
  void httpGet();
};

#endif