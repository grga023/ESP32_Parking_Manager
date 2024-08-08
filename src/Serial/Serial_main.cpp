#include "Serial_main.h"
#include "Servo_main.h"
#include "SetupAll.h"

extern bool EParkingSlots[4];
extern TaskHandle_t xTaskEntryRamp;
void SerialHandler(bool *EParkingSlots)
{
  std::string arrayString = "";
  for (int i = 0; i < 4; i++) {
      arrayString += EParkingSlots[i] ? "true " : "false ";
  }
  Serial.println(arrayString.c_str());
}

void SerialCommand()
{
  if (Serial.available() > 0) 
  {
    vTaskResume(xTaskEntryRamp);
    String command = Serial.readStringUntil('\n');
    if (command == "open") {
      EnterRamp();
    }
  }
}

void Task_SerialState(void* param)
{
  while(1)
  {
    SerialHandler(EParkingSlots);
    SerialCommand();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}