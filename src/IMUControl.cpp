#include "IMUControl.h"


void IMUControl::init() {
  if (!icm.init()) {
    Serial.println("ICM20948 init failed");
  } else {
    Serial.println("ICM20948 init success");
  }
  Serial.println("Caibrating IMU");
  icm.autoOffsets();
  delay(10);
  Serial.println("Calibration done");

  // Accelerometer Initialization
  icm.setAccRange(ICM20948_ACC_RANGE_2G);
  icm.setAccDLPF(ICM20948_DLPF_6);
  icm.setAccSampleRateDivider(10);

  // Gyroscope Initialization
  icm.setGyrRange(ICM20948_GYRO_RANGE_2000);
}
