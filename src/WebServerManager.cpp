#include "WebServerManager.h"
#include "WifiManager.h"
#include "DataTracker.h"
#include "SetupPage.h"

void WebServerManager::init(const DataTracker& dataTracker, CommandHandler& commandHandler) {
  handleRoot();
  handleUpdate(dataTracker);
  handleCommand(commandHandler);
  server.begin();
}

void WebServerManager::stop() {
  
}

void WebServerManager::handleRoot() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", setupPage);
  });
}

void WebServerManager::handleCommand(CommandHandler& commandHandler) {
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/command", [&commandHandler](AsyncWebServerRequest *request, JsonVariant &json) {
    JsonDocument data;
    if (json.is<JsonArray>())
    {
      data = json.as<JsonArray>();
      request->send(400, "application/json", "{\"status\": \"error\", \"message\": \"Arrays not supported\"}");
      return;
    }
    else if (json.is<JsonObject>())
    {
      data = json.as<JsonObject>();
    }
    else
    {
      request->send(400, "application/json", "{\"status\": \"error\", \"message\": \"Invalid JSON\"}");
      return;
    }
    commandHandler.handle(data);
    request->send(200, "application/json", "{\"status\": \"ok\"}");
  });
  server.addHandler(handler);

}

void WebServerManager::handleUpdate(const DataTracker& dataTracker) {
  server.on("/update", HTTP_GET, [&dataTracker](AsyncWebServerRequest *request) {
    std::string response;
    Serial.println("Preparing JSON");
    serializeJson(dataTracker.getJson(), response);
    Serial.println("Sending: " + String(response.c_str()));
    request->send(200, "application/json", response.c_str());
  });
}
