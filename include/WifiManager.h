#pragma once

#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <cstdint>
#include <vector>
#include <string>

struct SavedNetwork {
  std::string ssid;
  std::string password;

  SavedNetwork(const std::string& ssid, const std::string& password) : ssid(ssid), password(password) {}
};

struct ScannedNetwork {
  std::string ssid;
  std::int32_t rssi;
  std::uint8_t encryptionType;

  ScannedNetwork(const std::string& ssid, std::int32_t rssi, std::uint8_t encryptionType) : ssid(ssid), rssi(rssi), encryptionType(encryptionType) {}
};


class WifiManager {
public:
  void init();
  void stop();
  
  const std::vector<ScannedNetwork>& getScannedNetworks() const {return scannedNetworks;}
  const std::vector<SavedNetwork>& getSavedNetworks() const { return savedNetworks; }

  const std::size_t getSavedNetworksSize() const { return savedNetworks.size(); }
  const std::size_t getScannedNetworksSize() const { return scannedNetworks.size(); }

private:
  static constexpr const char* SSID = "ESP32";
  static constexpr const char* PASSWORD = "12345678";
  static constexpr const char* CONFIG_FILE = "/wifi_config.json";
  std::vector<SavedNetwork> savedNetworks;
  std::vector<ScannedNetwork> scannedNetworks;
  bool connected = false;

  void loadConfig(const char* file = CONFIG_FILE);
  void saveConfig();
  void scanNetworks();
  void connectToBestNetwork();

  bool addNetwork(const std::string& ssid, const std::string& password);
  bool removeNetwork(const std::string& ssid);
};


