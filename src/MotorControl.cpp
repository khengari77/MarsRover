#include "MotorControl.h"

                        
void MotorControl::init(){
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  ledcAttach(PWMA, FREQ, ANALOG_WRITE_BITS);

  ledcAttach(PWMB, FREQ, ANALOG_WRITE_BITS);

  MotorControl::stop();
}

void MotorControl::stop(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void MotorControl::leftMove(const float pwm){
  int pwmIntA = round(pwm);
  if (pwmIntA == 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    return;
  }

  // Determine the direction of rotation based on the pwmInput value
  if (pwmIntA > 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    // constrain() function limits pwmIntA value between MIN_PWM and MAX_PWM
    ledcWrite(PWMA, constrain(pwmIntA, MIN_PWM, MAX_PWM));
  } else {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(PWMA, constrain(-pwmIntA, MIN_PWM, MAX_PWM));
  }
}

void MotorControl::rightMove(const float pwm){
  int pwmIntB = round(pwm);

  if (pwmIntB == 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    return;
  }

  if (pwmIntB > 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(PWMB, constrain(pwmIntB, MIN_PWM, MAX_PWM));
  } else {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(PWMB, constrain(-pwmIntB, MIN_PWM, MAX_PWM));
  }
}



