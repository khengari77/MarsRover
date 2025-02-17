#pragma once

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

#include "DataTracker.h"
#include "CommandHandler.h"



class WebServerManager {
public:
  void init(const DataTracker &dataTracker, CommandHandler& commandHandler);
  void stop();

private:
  AsyncWebServer server{80};
  void handleRoot();
  void handleScan();
  void handleCommand(CommandHandler& commandHandler);
  void handleUpdate(const DataTracker& dataTracker);
};
