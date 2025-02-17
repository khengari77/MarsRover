#include "SpeedControl.h"


SpeedControl::SpeedControl(MotorControl& motorControl, EncoderControl& encoderLeft, EncoderControl& encoderRight) 
  : motorControl(motorControl),
    encoderLeft(encoderLeft),
    encoderRight(encoderRight),
    pid(kp, ki, kd, PID::Direct) {
  
    mutex = xSemaphoreCreateMutex();
    if (mutex == nullptr) {
      // Implement error handling (e.g., log and halt)
      Serial.println("Failed to create mutex");
      while (true); // Or use other mechanism to handle error
    }
}

void SpeedControl::init() {
  pid.Start(speed, output, setpoint);
  pid.SetOutputLimits(-255, 255);
  pid.SetMode(PID::Automatic);
  xTaskCreate(
      taskFunction,
      "SpeedControlTask",
      2048,  // Adjust stack size as needed
      this,
      1,     // Priority
      &taskHandle
  );
}

void SpeedControl::taskFunction(void* parameter) {
    SpeedControl* instance = static_cast<SpeedControl*>(parameter);
    const TickType_t period = pdMS_TO_TICKS(50); // 10ms period
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        instance->update();
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void SpeedControl::setSpeed(float speed) {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
    setpoint = speed;
    xSemaphoreGive(mutex);
  }
}

void SpeedControl::update() {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
   
    double leftDeltaTime = static_cast<double>(encoderLeft.getDeltaTime()) / 1000000.0;
    double rightDeltaTime = static_cast<double>(encoderRight.getDeltaTime()) / 1000000.0; // Convert microseconds to seconds for encoderRight.getDeltaTime();
    double leftDeltaCount = encoderLeft.getDeltaCount();
    double rightDeltaCount = encoderRight.getDeltaCount();
    double leftSpeed = (PULSE_RATE * leftDeltaCount) / leftDeltaTime;
    double rightSpeed = (PULSE_RATE * rightDeltaCount) / rightDeltaTime;
    double speed = sqrt(leftSpeed * leftSpeed + rightSpeed * rightSpeed);

    Serial.println("Speed: " + String(speed) + "Output: " + String(output) + "Setpoint: " + String(setpoint));

    if (setpoint != lastSetpoint) {
      pid.Setpoint(setpoint);
      lastSetpoint = setpoint;
    }
    output = pid.Run(speed);
    if (setpoint == 0) {
      output = 0;
      motorControl.stop();
    } else {
      motorControl.leftMove(output);
      motorControl.rightMove(output);
    }
    xSemaphoreGive(mutex);
  }
}


void SpeedControl::registerUpdates(DataTracker& dataTracker) {
    dataTracker.addValueCallback("speed", [this]() {
        xSemaphoreTake(mutex, portMAX_DELAY);
        float val = speed;
        xSemaphoreGive(mutex);
        return val;
    });

    dataTracker.addValueCallback("leftSpeed", [this]() {
        xSemaphoreTake(mutex, portMAX_DELAY);
        float val = leftSpeed;
        xSemaphoreGive(mutex);
        return val;
    });

    dataTracker.addValueCallback("rightSpeed", [this]() {
        xSemaphoreTake(mutex, portMAX_DELAY);
        float val = rightSpeed;
        xSemaphoreGive(mutex);
        return val;
    });
    dataTracker.addValueCallback("output", [this]() {
        xSemaphoreTake(mutex, portMAX_DELAY);
        float val = output;
        xSemaphoreGive(mutex);
        return val;
    });
    dataTracker.addValueCallback("setpoint", [this]() {
        xSemaphoreTake(mutex, portMAX_DELAY);
        float val = setpoint;
        xSemaphoreGive(mutex);
        return val;
    });
}
