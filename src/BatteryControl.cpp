#include "BatteryControl.h"


void BatteryControl::init(){
  if(!ina219.init()){
    Serial.println("INA219 not connected!");
  }
  ina219.setADCMode(BIT_MODE_9);
  ina219.setPGain(PG_320);
  ina219.setBusRange(BRNG_16);
  ina219.setShuntSizeInOhms(0.01); // used in INA219.
}

void BatteryControl::registerUpdates(DataTracker& dataTracker){
  dataTracker.addValueCallback("shuntVoltage_mV", [this](){ return ina219.getShuntVoltage_mV();});
  dataTracker.addValueCallback("busVoltage_V", [this](){ return ina219.getBusVoltage_V();});
  dataTracker.addValueCallback("current_mA", [this](){ return ina219.getCurrent_mA();});
  dataTracker.addValueCallback("power_mW", [this](){ return ina219.getBusPower();});
  dataTracker.addValueCallback("ina219_overflow", [this](){ return ina219.getOverflow();});
  dataTracker.addValueCallback("loadVoltage_V", [this](){ return ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV()/1000);});
}
