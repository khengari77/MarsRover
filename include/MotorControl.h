#pragma once
#include <Arduino.h>
#include <cstdint>

class MotorControl {
public:
  void init();
  void stop();
  void leftMove(const float pwm); 
  void rightMove(const float pwm);
private:
  const std::uint32_t FREQ = 100000;
  const std::uint8_t  channel_A = 5;
  const std::uint8_t channel_B = 6;


  const std::uint8_t PWMA = 25;         // Motor A PWM control  
  const std::uint8_t AIN2 = 17;         // Motor A input 2     
  const std::uint8_t AIN1 = 21;         // Motor A input 1     
  const std::uint8_t BIN1 = 22;         // Motor B input 1       
  const std::uint8_t BIN2 = 23;         // Motor B input 2       
  const std::uint8_t PWMB = 26;         // Motor B PWM control  
  
  const std::uint8_t ANALOG_WRITE_BITS = 8;
  const std::uint16_t MAX_PWM = pow(2, ANALOG_WRITE_BITS)-1;
  const std::uint16_t MIN_PWM = -MAX_PWM; 
};
