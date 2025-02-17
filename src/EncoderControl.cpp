// EncoderControl.cpp
#include "EncoderControl.h"
#include <cstdint>
#include "DataTracker.h"

EncoderControl::EncoderControl(int pinA, int pinB) {
  encoder.attachHalfQuad(pinA, pinB);
  mutex = xSemaphoreCreateMutex();
  if (mutex == nullptr) {
    // Implement error handling (e.g., log and halt)
    Serial.println("Failed to create mutex");
    while (true); // Or use other mechanism to handle error
  }
}

void EncoderControl::init(std::string _key) {
    encoder.setCount(0);
    key = _key;
    xTaskCreate(
        taskFunction,
        (std::string("EncoderTask_")+key).c_str(),
        2048,  // Adjust stack size as needed
        this,
        1,     // Priority
        &taskHandle
    );
}

void EncoderControl::taskFunction(void* parameter) {
    EncoderControl* instance = static_cast<EncoderControl*>(parameter);
    const TickType_t period = pdMS_TO_TICKS(10); // 10ms period
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        instance->update();
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void EncoderControl::update() {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        previousCount = currentCount;
        previousTime = currentTime;

        currentCount = encoder.getCount();
        currentTime = xTaskGetTickCount() * portTICK_PERIOD_MS;

        xSemaphoreGive(mutex);
    }
}

std::int32_t EncoderControl::getCurrentCount() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t count = currentCount;
    xSemaphoreGive(mutex);
    return count;
}
std::int32_t EncoderControl::getPreviousCount() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t count = previousCount;
    xSemaphoreGive(mutex);
    return count;
}
std::int32_t EncoderControl::getDeltaCount() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t count = currentCount - previousCount;
    xSemaphoreGive(mutex);
    return count;
}

std::uint32_t EncoderControl::getCurrentTime() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t time = currentTime;
    xSemaphoreGive(mutex);
    return time;
}
std::uint32_t EncoderControl::getPreviousTime() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t time = previousTime;
    xSemaphoreGive(mutex);
    return time;
}
std::uint32_t EncoderControl::getDeltaTime() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t time = currentTime - previousTime;
    xSemaphoreGive(mutex);
    return time;
}

void EncoderControl::registerUpdates(DataTracker& dataTracker) {
    dataTracker.addValueCallback(key + ".currentCount", [this]() {
        return getCurrentCount(); 
    });

    dataTracker.addValueCallback(key + ".previousCount", [this]() {
        return getPreviousCount();
    });

    dataTracker.addValueCallback(key + ".deltaCount", [this]() {
        return getDeltaCount();
    });

    dataTracker.addValueCallback(key + ".currentTime", [this]() {
        return getCurrentTime();
    });

    dataTracker.addValueCallback(key + ".previousTime", [this]() {
        return getPreviousTime();
    });
    dataTracker.addValueCallback(key + ".deltaTime", [this]() {
        return getDeltaTime();
    });
}

std::int32_t EncoderControl::getEncoderCount() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    std::uint32_t count = encoder.getCount();
    xSemaphoreGive(mutex);
    return count;
}

#if 0
#include "EncoderControl.h"
#include <cstdint>

EncoderControl::EncoderControl(int pinA, int pinB) {
  encoder.attachHalfQuad(pinA, pinB);
}

void EncoderControl::init() {
  encoder.setCount(0);
}

std::uint32_t EncoderControl::getEncoderCount() {
  return encoder.getCount();
}

void EncoderControl::update(const std::string& key, DataTracker& dataTracker) {
    // Capture current count and time
    std::uint32_t currentCount = static_cast<std::uint32_t>(encoder.getCount());
    std::uint32_t currentTime = millis();


    // Update previous values for next iteration
    previousCount = currentCount;
    previousTime = currentTime;
}

void EncoderControl::registerUpdates(std::string key, DataTracker& dataTracker) {
  dataTracker.addValueCallback(key + ".currentCount", [this]() { return currentCount; });
  dataTracker.addValueCallback(key + ".previousCount", [this]() { return previousCount; });
  dataTracker.addValueCallback(key + ".deltaCount", [this]() { return currentCount - previousCount; });
  dataTracker.addValueCallback(key + ".currentTime", [this]() { return currentTime; });
  dataTracker.addValueCallback(key + ".previousTime", [this]() { return previousTime; });
}

double plusesRate = 3.14159265359 * WHEEL_D / ONE_CIRCLE_PLUSES;



void getWheelSpeed() {
  unsigned long currentTime = micros();
  long encoderPulsesA = encoderA.getCount();
  long encoderPulsesB = encoderB.getCount();

  if (!SET_MOTOR_DIR) {
    speedGetA = (plusesRate * (encoderPulsesA - lastEncoderA)) / ((double)(currentTime - lastTime) / 1000000);
    speedGetB = (plusesRate * (encoderPulsesB - lastEncoderB)) / ((double)(currentTime - lastTime) / 1000000);
  } else {
    speedGetA = (plusesRate * (lastEncoderA - encoderPulsesA)) / ((double)(currentTime - lastTime) / 1000000);
    speedGetB = (plusesRate * (lastEncoderB - encoderPulsesB)) / ((double)(currentTime - lastTime) / 1000000);
  }
  lastEncoderA = encoderPulsesA;
  lastEncoderB = encoderPulsesB;
  lastTime = currentTime;
}

#endif
