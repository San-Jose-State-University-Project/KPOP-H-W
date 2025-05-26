#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <ApiClient.h>

class WebClient {
public:
  WebClient(ApiClient *client);
  void begin();
  void handleClient();
  void setApiClient(ApiClient *client);
  void setPageManager(PageManager *pm);
  String getApiUrl();
private:
  ESP8266WebServer server;
  String apiUrl;
  String endPoint;
  ApiClient *apiClient;
  PageManager *pageManager;
  void handleRoot();
  void handleSubmit();
  void handleSetEndpoint();
};

#endif