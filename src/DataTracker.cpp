#include "DataTracker.h"



void DataTracker::addValue(const std::string& key, const ValueType& value) {
  data[key] = value;
}

ValueType DataTracker::getValue(const std::string& key) {
  return data[key];
}

void DataTracker::addValueCallback(const std::string& key, ValueCallback callback) {
  valueCallbacks[key] = callback;
  Serial.println("Added callback for: " + String(key.data()));
  data[key] = callback();
}

void DataTracker::update() {
  for (const auto& [key, callback] : valueCallbacks) {
    data[key] = callback();
  }
}

const JsonDocument DataTracker::getJson() const {
  JsonDocument doc;
  for (const auto& [key, value] : data) {
    doc[key] = value;
  }
  return doc;
}


