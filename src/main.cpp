#include <Arduino.h>
#include <string>
#include <Wire.h>
#include <SCServo.h>

#include "WifiManager.h"
#include "DataTracker.h"
#include "WebServerManager.h"
#include "CommandHandler.h"
#include "BatteryControl.h"
#include "MotorControl.h"
#include "EncoderControl.h"
#include "SpeedControl.h"
#include "SteeringControl.h"


#define S_SCL   33
#define S_SDA   32

#define S_RXD 18
#define S_TXD 19

#define AENCA 35        // Encoder A input      
#define AENCB 34
#define BENCA 27
#define BENCB 16


SCSCL sc;

WifiManager wifiManager;
DataTracker dataTracker;
WebServerManager webServerManager;
CommandHandler commandHandler;
BatteryControl batteryControl;
MotorControl motorControl;
EncoderControl encoderA(AENCA, AENCB);
EncoderControl encoderB(BENCA, BENCB);
SpeedControl speedControl(motorControl, encoderA, encoderB);
SteeringControl steeringControl(6, sc);

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize Serial1 & attch to servo
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  sc.pSerial = &Serial1;
  while(!Serial1) {}

  // Initialize I2C
  Wire.begin(S_SDA, S_SCL);

  // Initialize WiFi
  wifiManager.init();
  
  // Initialize WebServer
  webServerManager.init(dataTracker, commandHandler);
  
  batteryControl.init();
  batteryControl.registerUpdates(dataTracker);

  motorControl.init();

  encoderA.init("encoderA");
  encoderA.registerUpdates(dataTracker);
  
  encoderB.init("encoderB");
  encoderB.registerUpdates(dataTracker);

  speedControl.init();
  speedControl.registerUpdates(dataTracker);

  steeringControl.init();

  commandHandler.addHandler("speed", [](float speed){
      if (abs(speed) > 2) {
        speed = (speed / abs(speed)) * 2;
      }

      speedControl.setSpeed(speed);
  });

  commandHandler.addHandler("angle", [](float angle){
      steeringControl.setAngle(static_cast<std::uint16_t>(angle));
  });


}

void loop() {
  dataTracker.update();
  vTaskDelay(10/portTICK_PERIOD_MS);
}
