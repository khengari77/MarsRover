#pragma once

#include <INA219_WE.h>
#define INA219_ADDRESS 0x42

#include "DataTracker.h"

class BatteryControl {
public:
  void init();
  void registerUpdates(DataTracker& dataTracker);
private:
  INA219_WE ina219 = INA219_WE(INA219_ADDRESS);
};

