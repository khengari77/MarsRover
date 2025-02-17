#include "CommandHandler.h"

CommandHandler::CommandHandler() {
  revertTimer = xTimerCreate(
    "RevertTimer",         // Timer name
    pdMS_TO_TICKS(1000),   // Default 1s (adjust dynamically)
    pdFALSE,               // One-shot timer
    this,                  // Pass 'this' to callback
    revertTimerCallback    // Timer expiry handler
  );
  isCommandActive = false;
}

void CommandHandler::addHandler(const std::string& key, HandlerCallback callback) {
  callbacks[key] = callback;
}

  // Timer callback (static to match FreeRTOS signature)
void CommandHandler::revertTimerCallback(TimerHandle_t xTimer) {
  CommandHandler* handler = static_cast<CommandHandler*>(pvTimerGetTimerID(xTimer));
  handler->revertToDefaultState();
}

void CommandHandler::revertToDefaultState() {
  // Reset hardware/state to default here
  isCommandActive = false;
  for (const auto& [key, callback] : callbacks) {
    callback(0);
  }
  Serial.println("Reverted to default state");
}

void CommandHandler::handle(const JsonDocument& data) {
  // 1. Stop timer if already running
  xTimerStop(revertTimer, 0);

  for (const auto& [key, callback] : callbacks) {
    if (!data.containsKey(key) || !data[key].is<float>()) {
      continue;
    }
    callback(data[key].as<float>());
  }

  // 3. Update state and restart timer with new duration
  if (data.containsKey("duration")) {
    int durationSeconds = data["duration"];
    xTimerChangePeriod(revertTimer, pdMS_TO_TICKS(durationSeconds * 1000), 0);
  }
  xTimerStart(revertTimer, 0); // Start the timer
  isCommandActive = true;
}

