#include "SteeringControl.h"
#include <vector>


SteeringControl::SteeringControl(const std::uint8_t N, SCSCL& sc) 
  : N_SERVO(N), 
    sc(sc) {
  for (int i = 1; i <= N_SERVO; i++) {
    ID.emplace_back(i);
    servo_setpoint.emplace_back(0);
  }
  mutex = xSemaphoreCreateMutex();
  if (mutex == nullptr) {
    // Implement error handling (e.g., log and halt)
    Serial.println("Failed to create mutex");
    while (true); // Or use other mechanism to handle error
  }
}

void SteeringControl::taskFunction(void* parameter) {
    SteeringControl* instance = static_cast<SteeringControl*>(parameter);
    const TickType_t period = pdMS_TO_TICKS(50); // 10ms period
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        instance->update();
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void SteeringControl::init() {
  for (int i = 0; i < N_SERVO; i++) {
    Serial.println("Reached servo init");
    sc.WritePos(ID.at(i), offset.at(i), 0, 1500);
  }
  xTaskCreate(
      taskFunction,
      "SteeringControlTask",
      2048,  // Adjust stack size as needed
      this,
      1,     // Priority
      &taskHandle
  );
}

void SteeringControl::setAngle(std::uint16_t angle) {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    angleSetpoint = angle;
    xSemaphoreGive(mutex);
  }
}

void SteeringControl::update() {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    computeAngles(angleSetpoint);
    writeAngles();
    xSemaphoreGive(mutex);
  }
}

const std::vector<std::uint16_t> SteeringControl::getOffsetAngles() {
  std::vector<std::uint16_t> offsetAngles;
  for (auto o : offset) {
    offsetAngles.emplace_back(map(o, 1 << 10, 0, 300, 0));
  }
  return offsetAngles;
}

void SteeringControl::computeAngles(std::int8_t angle) {
  angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);
  double turn_radius = LENGTH / ((abs(angle) / 360.0) * 2 * M_PI);
  std::int8_t left_angle = 0, right_angle = 0;
  if (angle < 0) {
    left_angle = angle;
    right_angle = -LENGTH / (2 * M_PI * (turn_radius + WIDTH)) * 360;   
  } else {
    right_angle = angle;
    left_angle = LENGTH / (2 * M_PI * (turn_radius + WIDTH)) * 360;   

  }
  std::vector<std::uint16_t> offsetAngles = getOffsetAngles(); 
  servo_setpoint.at(0) = offsetAngles.at(0) - right_angle;
  servo_setpoint.at(1) = offsetAngles.at(1) - left_angle;
  servo_setpoint.at(2) = offsetAngles.at(2) - 0.78 * right_angle;
  servo_setpoint.at(3) = offsetAngles.at(3) - 0.78 * left_angle;
  servo_setpoint.at(4) = offsetAngles.at(4) + right_angle;
  servo_setpoint.at(5) = offsetAngles.at(5) + left_angle;

}

void SteeringControl::writeAngle(std::uint8_t id, std::uint16_t angle) {
  angle = map(angle, 0, 300, 1 << 10, 0);
  sc.WritePos(id, angle, 0, 1500);
}

void SteeringControl::writeAngles() {
  for (auto i : ID) {
    writeAngle(i, servo_setpoint.at(i - 1));
  }
}

std::uint16_t SteeringControl::readAngle(std::uint8_t id) {
  return map(sc.ReadPos(id), 1 << 10, 0, 300, 0);
}
