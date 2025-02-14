#include <Arduino.h>
#include <string>

#include "WifiManager.h"
#include "DataTracker.h"
#include "WebServerManager.h"
#include "CommandHandler.h"
#include "BatteryControl.h"

WifiManager wifiManager;
DataTracker dataTracker;
WebServerManager webServerManager;
CommandHandler commandHandler;
BatteryControl batteryControl;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  wifiManager.init();
  webServerManager.init(dataTracker, commandHandler);
  batteryControl.init();
  batteryControl.registerUpdates(dataTracker); 

}

void loop() {
  dataTracker.update();
  delay(10);
}
