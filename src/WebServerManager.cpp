#include "WebServerManager.h"
#include "WifiManager.h"
#include "DataTracker.h"
#include "SetupPage.h"

void WebServerManager::init(const DataTracker& dataTracker, const CommandHandler& commandHandler) {
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

void WebServerManager::handleCommand(const CommandHandler& commandHandler) {
  server.on("/command", HTTP_POST, [&commandHandler](AsyncWebServerRequest *request) {
    JsonDocument doc;
    if (request->contentType() == "application/json") {
      // Deserialize directly from the request body
      DeserializationError error = deserializeJson(doc, request->arg("plain"));
      if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        request->send(400, "text/plain", "JSON parse failed");
      }
      else {
        commandHandler.handle(doc);
      }
    } else {
      request->send(400, "text/plain", "Unsupported content type");
    } 
  });
}

void WebServerManager::handleUpdate(const DataTracker& dataTracker) {
  server.on("/update", HTTP_GET, [&dataTracker](AsyncWebServerRequest *request) {
    std::string response;
    serializeJson(dataTracker.getData(), response);
    Serial.println("Sending: " + String(response.c_str()));
    request->send(200, "application/json", response.c_str());
  });
}
