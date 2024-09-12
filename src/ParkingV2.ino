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
#include "WebServer.h"
#include "CPU_monitoring.h"
#include "FlashDriver.h"

TaskHandle_t xTaskScanRFID;
TaskHandle_t xTaskDisplayState;
TaskHandle_t xTaskDistanceSensor;
TaskHandle_t xTaskExitRampTrigger;
TaskHandle_t xTaskSerial;
TaskHandle_t xTaskEntryRamp;
TaskHandle_t xTaskExitRamp;

int empty = 0;
int sum[4] = {0}; 
bool EParkingSlots[4] = {true,true,true,true};
persons ljudi;
components componentInit = {false};

void setup() {
  Serial.begin(115200);
  delay(2000);

  readPersonsFromFlash_4K(ljudi);

  perfmon_start();
  WIFIsetup();
  setupWebServer();

  setupServo();
  delay(200);
  setupRFID();
  setupDisplay();
  VL_Setup();
  delay(200);

  #ifdef DEBUG
    Serial.println("ESP32 Clock Data:");
    Serial.println("==================================");
    Serial.printf("CPU Frequency: %u MHz\n", getCpuFrequencyMhz());
    Serial.printf("APB Frequency: %u Hz\n", getApbFrequency());
    Serial.printf("I2C Bus Frequency: %u Hz\n", I2C_SPEED);
    Serial.println("==================================");
    
    Serial.println();
    
    Serial.println("Components init Data:");
    Serial.println("==================================");
    Serial.println("Distance sensor init data:");
    Serial.println("[1]: " + componentInit.distance[0] ? "Starded" : "Faild to start");
    Serial.println("[2]: " + componentInit.distance[1] ? "Starded" : "Faild to start");
    Serial.println("[3]: " + componentInit.distance[2] ? "Starded" : "Faild to start");
    Serial.println("[4]: " + componentInit.distance[3] ? "Starded" : "Faild to start");
    Serial.println("[5]: " + componentInit.distance[4] ? "Starded" : "Faild to start");
    Serial.println("==================================");
    Serial.print("RFID scanner init state:");
    Serial.println(componentInit.rfid ? " Starded" : " Faild to start");
    Serial.println("==================================");
    Serial.print("Display init state: ");
    Serial.println(componentInit.display ? " Starded" : " Faild to start");
    Serial.println("==================================");
    Serial.println();

    for (int i = 0; i < MAX; i++) {
        Serial.print("Person ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(ljudi[i].name);
        Serial.print(" - Data: ");
        for (int j = 0; j < 4; j++) {
            Serial.printf("%02X ", ljudi[i].personId[j]);
        }
        Serial.println();
    }
#endif

  if(componentInit.rfid)
    xTaskCreate(Task_RFIDScanner, "RFIDScanner", 4096, NULL, 1, &xTaskScanRFID);
  if(componentInit.display)
    xTaskCreate(Task_DisplayState, "DisplayState", 4096, NULL, 1, &xTaskDisplayState);
  if(componentInit.distance[0] || componentInit.distance[1] || componentInit.distance[2] || componentInit.distance[3])  
    xTaskCreate(Task_ParkingSlotCheck, "ParkingSlotCheck", 2048, NULL, 1, &xTaskDistanceSensor); 
  if(componentInit.distance[4])
    xTaskCreate(Task_ExitRamp, "ExitRampTrigger", 2048, NULL, 1, &xTaskExitRampTrigger);

  xTaskCreate(task_entry, "EntryRamp", 1024, NULL, 2, &xTaskEntryRamp);  
  xTaskCreate(task_exit, "ExitRamp", 1024, NULL, 2, &xTaskExitRamp);  

  #ifdef DEBUG
    xTaskCreate(Task_SerialState, "SerialTask", 1024, NULL, 0, &xTaskSerial); 
  #endif
}

void loop() 
{

}
