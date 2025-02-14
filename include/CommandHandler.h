#pragma once

#include <ArduinoJson.h>

#include <string>
#include <functional>
#include <unordered_map>

using HandlerCallback = std::function<void(float)>;


class CommandHandler {
public:
  void init();
  void addHandler(const std::string& key, HandlerCallback callback);
  void handle(const JsonDocument& data) const;

private:
    std::unordered_map<std::string, HandlerCallback> callbacks;
};
