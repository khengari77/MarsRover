#pragma once

#include <Arduino.h>

class HeartBeatControl {
public:
    HeartBeatControl();
    void init();
    void reset(std::uint32_t time);
private:
    std::uint32_t lastHeartBeat = 0;
    std::uint32_t currentHeartBeat = 0;
};
