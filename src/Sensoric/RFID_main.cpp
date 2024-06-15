#include <Wire.h>
#include "SetupAll.h"
#include <Adafruit_PN532.h>
#include "RFID_main.h"
#include "Display_main.h"
#include "Servo_main.h"
#include "Proximity_main.h"

Adafruit_PN532 scanner(SDA_PIN, SCL_PIN);

extern int empty;
extern persons ljudi;
extern components componentInit;
extern TaskHandle_t xTaskDisplayState;

void setupRFID()
{
  scanner.begin();
  uint32_t versiondata = scanner.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("RFID module is not connected.");
    componentInit.rfid = false;
    return;
  }
  componentInit.rfid = true;
  scanner.SAMConfig();
}

void Task_RFIDScanner(void* param)
{
  uint8_t scanned;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
  uint8_t uidLength;

  while(1)
  {
    if(!empty){
      scanner.wakeup();
      scanned = scanner.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 200);
      if(scanned)
      {
        (void)checkID(uid, ljudi);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void openRamp()
{
  if (triggerDistance()){
    ExitRamp();
  }
  else{
    //do nothing
  }
}

person checkID(uint8_t* uid, person* ljudi)
{
  person EmptyPerson = {0};
  for (int i = 0; i < MAX; i++) {
      bool match = true;
      for (int m = 0; m < 4; m++) {
          if (ljudi[i].personId[m] != uid[m]) {
              match = false;
              break;
          }
      }
      if (match) {
        vTaskSuspend(xTaskDisplayState);
        WriteOnDisplay(3, 0, 0, "HELLO!", true);
        displayContent();
        vTaskDelay(pdMS_TO_TICKS(2000));
        WriteOnDisplay(3, 5, 20, ljudi[i].name.c_str(), true);
        displayContent();
        EnterRamp();
        vTaskResume(xTaskDisplayState);
        return ljudi[i];
      }
  }
  vTaskSuspend(xTaskDisplayState);
  WriteOnDisplay(3, 0, 0, "ACCESS DENIED", true);
  displayContent();
  vTaskDelay(pdMS_TO_TICKS(2000));
  vTaskResume(xTaskDisplayState);
  return EmptyPerson;
}

// void scanRFID(int empty, person* ljudi )
// {
//   uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; 
//   uint8_t scanned;
//   uint8_t uidLength;
//   std::string emptySlots = "";
//   person Person;

//   if (empty != 0)
//   {
//     WriteOnDisplay(2, 5, 0, "FREE SPOTS", true);
//     emptySlots = std::to_string(empty);
//     WriteOnDisplay(4, 25, 25, emptySlots, false);
//     WriteOnDisplay(4, 48, 25, "/4", false);
//     displayContent();
//     scanner.wakeup();
//     scanned = scanner.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 200);
//     if (scanned) 
//     {
//       Person = checkID(uid, ljudi);
//       if (Person.name != "")
//       {
//         WriteOnDisplay(3, 0, 0, "HELLO!", true);
//         displayContent();
//         vTaskDelay(pdMS_TO_TICKS(2000));
//         WriteOnDisplay(3, 5, 20, Person.name.c_str(), true);
//         displayContent();
//         EnterRamp();
//       }
//       else
//       {
//         WriteOnDisplay(3, 0, 0, "ACCESS DENIED", true);
//         displayContent();
//         vTaskDelay(pdMS_TO_TICKS(2000));
//       }
//     }
//   }
//   else
//   {
//     WriteOnDisplay(3, 0, 0, "PARKING FULL", true);
//     displayContent();
//   }
// }