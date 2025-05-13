#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <ESP8266WebServer.h>

#include "ApiClient.h"

class WebClient {
public:
  WebClient();
  void begin();
  void handleClient();
  void setApiClient(ApiClient *client);
  String getApiUrl();

private:
  ESP8266WebServer server;
  String apiUrl;
  ApiClient *apiClient;
  void handleRoot();
  void handleSubmit();
};

#endif