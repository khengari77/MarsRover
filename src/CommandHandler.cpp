#include "CommandHandler.h"

void CommandHandler::addHandler(const std::string& key, HandlerCallback callback) {
  callbacks[key] = callback;
}

void CommandHandler::handle(const JsonDocument& data) const {
  for (const auto& [key, callback] : callbacks) {
    if (!data.containsKey(key) || !data[key].is<float>()) {
      continue;
    }
    callback(data[key].as<float>());
  }
}
