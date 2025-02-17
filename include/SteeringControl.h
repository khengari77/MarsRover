#pragma once
#include <Arduino.h>
#include <SCServo.h>
#include <vector>

#include <cstdint>

class SteeringControl {
public:
  SteeringControl(const std::uint8_t N, SCSCL& sc);
  void init();
  void update();

  void setAngle(std::uint16_t angle);

private:
  SemaphoreHandle_t mutex;
  TaskHandle_t taskHandle = nullptr;
  static void taskFunction(void* parameter);
  
  const std::int8_t MAX_ANGLE = 60;
  const std::int8_t MIN_ANGLE = -60;
  const std::uint8_t LENGTH = 41;
  const std::uint8_t WIDTH = 28;

  SCSCL& sc;

  std::uint8_t N_SERVO;
  std::vector<std::uint8_t> ID;
  const std::vector<std::uint16_t> offset = {1 << 9, 1 << 9, 1 << 9, 1 << 9, 1 << 9, 1 << 9};
  std::vector<std::uint16_t> servo_setpoint;

  std::int16_t angleSetpoint = 0;
  
  void computeAngles(std::int8_t angle);

  void writeAngle(std::uint8_t id, std::uint16_t angle);
  void writeAngles();

  std::uint16_t readAngle(std::uint8_t id);

  const std::vector<std::uint16_t> getOffsetAngles();
};

