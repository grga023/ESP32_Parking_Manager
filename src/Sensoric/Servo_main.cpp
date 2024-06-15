#include "SetupAll.h"
#include "Servo_main.h"
#include <Arduino.h>
#include <pwmWrite.h>


Pwm Servo = Pwm();
Pwm ServoExit = Pwm();
extern components componentInit;

void setupServo() 
{
  Servo.writeServo(ENTRY_RAMP_PIN, 80);
  Servo.writeServo(EXIT_RAMP_PIN, 80);
}

void EnterRamp() {
  for (int pos = 80; pos >= 0; pos--) {
    Servo.writeServo(ENTRY_RAMP_PIN, pos);
    delay(8); 
  }
  vTaskDelay(pdMS_TO_TICKS(OPEN_RAMP_DELAY));
  for (int pos = 0; pos <= 80; pos++) {
    Servo.writeServo(ENTRY_RAMP_PIN, pos);
    delay(8); 
  }
}

void ExitRamp() {
  for (int pos = 80; pos >= 0; pos--) {
    Servo.writeServo(EXIT_RAMP_PIN, pos);
    delay(8); 
  }
  vTaskDelay(pdMS_TO_TICKS(OPEN_RAMP_DELAY));
  for (int pos = 0; pos <= 80; pos++) {
    Servo.writeServo(EXIT_RAMP_PIN, pos);
    delay(8);
  }
}
