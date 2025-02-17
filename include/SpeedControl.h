#pragma once

#include <Arduino.h>
#include <PID_v2.h>
#include <cstdint>

#include "MotorControl.h"
#include "EncoderControl.h"

class SpeedControl {
public:
  SpeedControl(MotorControl& motorControl, EncoderControl& encoderLeft, EncoderControl& encoderRight);
  void init();
  void update();
  void registerUpdates(DataTracker& dataTracker);
  float getSpeed();
  float getRightSpeed();
  float getLeftSpeed();
  void setSpeed(float speed);


private:
  SemaphoreHandle_t mutex;
  TaskHandle_t taskHandle = nullptr;
  static void taskFunction(void* parameter);

  float leftSpeed = 0;
  float rightSpeed = 0;
  float speed = 0;

  float output = 0;
  float setpoint = 0;
  float lastSetpoint = 0;

  const float kp = 10;
  const float ki = 1000;
  const float kd = 0.0;

  PID_v2 pid;


  const double WHEEL_CIRCUMFERENCE = 0.315;
  const double CPR = 1000;
  const double PULSE_RATE = WHEEL_CIRCUMFERENCE / CPR;

  const std::int16_t MAX_PWM = 255;
  const std::int16_t MIN_PWM = -255;

  MotorControl& motorControl;
  EncoderControl& encoderLeft;
  EncoderControl& encoderRight;
};

