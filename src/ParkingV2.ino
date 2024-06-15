#include "RFID_main.h"
#include "SetupAll.h"
#include <Arduino.h>
#include "Display_main.h"
#include "Servo_main.h"
#include "Proximity_main.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Serial_main.h"
#include "esp32-hal-cpu.h"
#include "DistanceSensor.h"
#include "WiFi_conector.h"
#include "WiFi.h"
#include "DataSender.h"
#include "WebServer.h"

TaskHandle_t xTaskScanRFID;
TaskHandle_t xTaskDisplayState;
TaskHandle_t xTaskDistanceSensor;
TaskHandle_t xTaskExitRamp;
TaskHandle_t xTaskSerial;
TaskHandle_t xenter;
TaskHandle_t xexit;

int empty = 4;
int sum[4] = {0}; 
bool EParkingSlots[4] = {0};

void task_entry(void* param){
  while (1)
  {
    EnterRamp();
    vTaskSuspend(NULL);
  }
}

void task_exit(void* param){
  while (1)
  {
    ExitRamp();
    vTaskSuspend(NULL);
  }
}

// Za brisanje
person blue = {{74, 9 ,136, 116}, "BLUE"};
person white  = {{74, 42, 2, 116}, "WHITE"};
person yellow = {{74, 33 ,22, 116}, "YELLOW"};
person grey = {{74, 16, 196, 116}, "GREY"};
person orange  = {{74, 9 , 148, 116}, "ORANGE"};

persons ljudi = {blue, white, yellow, grey, orange};

components componentInit = {false};

void setup() {
  Serial.begin(115200);
  delay(2000);
  WIFIsetup();
  setupWebServer();

  setupServo();
  VL_Setup();
  setupRFID();
  setupDisplay();
  Serial.println("ESP32 Clock Data:");
  Serial.println("==================================");
  Serial.printf("CPU Frequency: %u MHz\n", getCpuFrequencyMhz());
  Serial.printf("APB Frequency: %u Hz\n", getApbFrequency());
  Serial.printf("I2C Bus Frequency: %u Hz\n", I2C_SPEED);
  Serial.println("==================================");
  
  Serial.println();
  
  Serial.println("Components init Data:");
  Serial.println("==================================");
  Serial.print("Distance sensor init data:");
  Serial.print(componentInit.distance[0] ? " Starded" : " faild to start");
  Serial.print(" " + componentInit.distance[1] ? " Starded" : " faild to start");
  Serial.print(" " + componentInit.distance[2] ? " Starded" : " faild to start");
  Serial.print(" " + componentInit.distance[3] ? " Starded" : " faild to start");
  Serial.println(" " + componentInit.distance[4] ? " Starded" : " faild to start");
  Serial.print("RFID scanner init state:");
  Serial.println(componentInit.rfid ? " Starded" : "faild to start");
  Serial.print("Display init state: ");
  Serial.println(componentInit.display ? " Starded" : "faild to start");
  Serial.println("==================================");
  Serial.println();

  if(componentInit.rfid)
    xTaskCreate(Task_RFIDScanner, "RFIDScanner", 4096, NULL, 1, &xTaskScanRFID);
  if(componentInit.display)
    xTaskCreate(Task_DisplayState, "DisplayState", 1024, NULL, 1, &xTaskDisplayState);
  if(componentInit.distance[0])
    xTaskCreate(Task_ParkingSlotCheck, "ParkingSlotCheck", 4096, NULL, 1, &xTaskDistanceSensor);
  if(componentInit.distance[4])
    xTaskCreate(Task_ExitRamp, "ExitRamp", 4096, NULL, 1, &xTaskExitRamp);
  xTaskCreate(Task_SerialState, "SerialTask", 4096, NULL, 0, &xTaskSerial); 

  xTaskCreate(task_entry, "SerialTask", 4096, NULL, 0, &xenter);  
  xTaskCreate(task_exit, "SerialTask", 4096, NULL, 0, &xexit);  
}

void loop() {

  // sendEParkingSlotsToVercel(EParkingSlots);
  delay(500);
}
