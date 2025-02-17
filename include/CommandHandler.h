#pragma once

#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include <string>
#include <functional>
#include <unordered_map>

using HandlerCallback = std::function<void(float)>;


class CommandHandler {
public:
  CommandHandler();
  void init();
  void addHandler(const std::string& key, HandlerCallback callback);
  void handle(const JsonDocument& data);
  static void revertTimerCallback(TimerHandle_t xTimer);
  void revertToDefaultState();

private:
    std::unordered_map<std::string, HandlerCallback> callbacks;
    TimerHandle_t revertTimer; // Timer to revert state
    bool isCommandActive;      // Track active command
};

