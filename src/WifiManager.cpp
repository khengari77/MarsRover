#include "WifiManager.h"

void WifiManager::init() {
  WiFi.mode(WIFI_STA);
  SPIFFS.begin();
  loadConfig();
  scanNetworks();
  connectToBestNetwork();
  std::size_t nSavedNetworks = savedNetworks.size();
  if (!connected) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(SSID, PASSWORD);
    Serial.println("AP mode");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
  }
  WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
       scanNetworks();
       connectToBestNetwork();
      }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}


void WifiManager::loadConfig(const char* file) {
  File configFile = SPIFFS.open(file, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }
  
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  } 

  JsonArray networks = doc.as<JsonArray>();
  for (JsonVariant network : networks) {
    savedNetworks.emplace_back(network["ssid"].as<std::string>(), network["password"].as<std::string>());
  }
  
  configFile.close();
  
}

void WifiManager::saveConfig() {
  File configFile = SPIFFS.open(CONFIG_FILE, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }
  JsonDocument doc;
  JsonArray networks = doc.as<JsonArray>();
  for (const auto& network : savedNetworks) {
    JsonObject networkObject = networks.add<JsonObject>();
    networkObject["ssid"] = network.ssid;
    networkObject["password"] = network.password;
  }
  serializeJson(doc, configFile);
  configFile.close();
}

void WifiManager::scanNetworks() {
  std::size_t n = WiFi.scanNetworks();
  scannedNetworks.clear();
  for (std::size_t i = 0; i < n; ++i) {
    scannedNetworks.emplace_back(std::string(WiFi.SSID(i).c_str()), WiFi.RSSI(i), WiFi.encryptionType(i));
  }
}

void WifiManager::connectToBestNetwork() {
  std::sort(scannedNetworks.begin(), scannedNetworks.end(), [](const ScannedNetwork& a, const ScannedNetwork& b) {
    return a.rssi > b.rssi;
  });


  for (const auto& network : scannedNetworks) {
    for (const auto& savedNetwork : savedNetworks) {
      int i = 0;
      if (network.ssid == savedNetwork.ssid) {
        WiFi.begin(savedNetwork.ssid.c_str(), savedNetwork.password.c_str());
        Serial.println("Connecting to " + String(savedNetwork.ssid.c_str()));
        while (WiFi.status() != WL_CONNECTED) {
          Serial.print(".");
          delay(500);
          i++;
          if (i > 100) {
            Serial.println("Failed to connect to " + String(savedNetwork.ssid.c_str()));
            break;
          }
        }
        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("Connected to " + String(savedNetwork.ssid.c_str()));
          connected = true;
          return;
        }
      }
    }
  }
  Serial.println("Failed to connect to any network");
  connected = false;
}
