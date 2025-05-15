#include "WebClient.h"

WebClient::WebClient(ApiClient *client) : server(80), apiUrl("") {
  apiClient = client;
}

void WebClient::begin() {
  server.on("/", [this](){handleRoot();});
  server.on("/submit", [this](){handleSubmit();});
  server.on("/set-endpoint", [this](){handleSetEndpoint();});
  server.begin();
  Serial.println("[WebClient] Server started");
  Serial.print("[WebClient] IP: ");
  Serial.println(WiFi.localIP());  // 웹서버 주소 콘솔에 출력
}

void WebClient::handleClient() {
  server.handleClient();
}

void WebClient::handleRoot() {
  String html = R"=====(
        <html>
        <head><meta charset="UTF-8"><title>API 설정</title></head>
        <body>
            <h2>API 주소 설정</h2>
            <form action="/submit" method="GET">
                <label for="api">API URL:</label><br>
                <input type="text" id="api" name="api" style="width:300px"><br><br>
                <input type="submit" value="제출">
            </form>
            <hr>
            <h2>API 엔드포인트 설정</h2>
            <form action="/set-endpoint" method="GET">
                <label for="ep">Endpoint:</label><br>
                <input type="text" id="ep" name="ep" style="width:300px"><br><br>
                <input type="submit" value="엔드포인트 설정">
            </form>
        </body>
        </html>
    )=====";
  server.send(200, "text/html", html);
}

void WebClient::setApiClient(ApiClient *client) {
  apiClient = client;
}

void WebClient::setPageManager(PageManager *pm) {
  pageManager = pm;
}


void WebClient::handleSubmit() {
  if (server.hasArg("api")) {
    apiUrl = server.arg("api");
    if (!apiUrl.startsWith("http://") && !apiUrl.startsWith("https://")) {
      apiUrl = "https://" + apiUrl;
    }
    apiClient->setApiUrl((char*)apiUrl.c_str());
    pageManager->clearPages();
    Serial.println("[WebClient] API URL 저장: " + apiUrl);

    String html = R"====(
      <html>
        <head>
          <meta http-equiv="refresh" content="2; url=/" />
        </head>
        <body>
          <h3>API 주소가 설정되었습니다.</h3>
          <p>잠시 후 메인 페이지로 이동합니다.</p>
        </body>
      </html>
    )====";

    server.send(200, "text/html", html);
  } else {
    server.send(400, "text/plain", "잘못된 요청");
  }
}

void WebClient::handleSetEndpoint() {
  if (server.hasArg("ep")) {
    endPoint = server.arg("ep");
    apiClient->setEndPoint((char*)endPoint.c_str());
    Serial.println("[WebClient] 엔드포인트 저장: " + endPoint);

    String html = R"====(
      <html>
        <head>
          <meta http-equiv="refresh" content="2; url=/" />
        </head>
        <body>
          <h3>엔드포인트가 설정되었습니다.</h3>
          <p>잠시 후 메인 페이지로 이동합니다.</p>
        </body>
      </html>
    )====";
    server.send(200, "text/html", html);
  } else {
    server.send(400, "text/plain", "잘못된 요청");
  }
}

String WebClient::getApiUrl() {
  return apiUrl;
}



