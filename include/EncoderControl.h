#pragma once

// EncoderControl.h
#include <Arduino.h>
//#include <FreeRTOS.h>
//#include <semphr.h>
#include <ESP32Encoder.h>

#include "DataTracker.h"

class EncoderControl {
private:
    ESP32Encoder encoder;
    SemaphoreHandle_t mutex;
    TaskHandle_t taskHandle = nullptr;
    static void taskFunction(void* parameter);

    std::string key;
    std::uint32_t currentCount = 0;
    std::uint32_t previousCount = 0;
    std::uint32_t currentTime = 0;
    std::uint32_t previousTime = 0;

public:
    EncoderControl(int pinA, int pinB);
    void init(std::string key);
    std::int32_t getEncoderCount();

    std::int32_t getCurrentCount();
    std::int32_t getPreviousCount();
    std::int32_t getDeltaCount();
    std::uint32_t getCurrentTime();
    std::uint32_t getPreviousTime();
    std::uint32_t getDeltaTime();
    
    std::string getKey();
    void update();
    void registerUpdates(DataTracker& dataTracker);
};

#if 0
#include <Arduino.h>
#include <ESP32Encoder.h>
#include <cstdint>
#include <string>


class EncoderControl {
  public:
    EncoderControl(int pinA, int pinB);
    void init();
    std::uint32_t getEncoderCount();
    void update();
    void registerUpdates(std::string key, DataTracker& dataTracker);

  private:
    ESP32Encoder encoder;
    std::uint32_t currentTime = 0;
    std::uint32_t currentCount = 0;
    std::uint32_t previousTime = 0;
    std::uint32_t previousCount = 0;

};

#endif
