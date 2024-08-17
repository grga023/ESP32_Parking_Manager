#include "Adafruit_VL6180X.h"
#include "SetupAll.h"
#include "DistanceSensor.h"
#include "Servo_main.h"

Adafruit_VL6180X vl1 = Adafruit_VL6180X();
Adafruit_VL6180X vl2 = Adafruit_VL6180X();
Adafruit_VL6180X vl3 = Adafruit_VL6180X();
Adafruit_VL6180X vl4 = Adafruit_VL6180X();
Adafruit_VL6180X vl5 = Adafruit_VL6180X();

extern TaskHandle_t xTaskExitRamp;

uint8_t SHDN[5]                 = {SHDN_VL1, SHDN_VL2, SHDN_VL3, SHDN_VL4, SHDN_VL5};
uint8_t VL_Address[5]           = {VL1_ADDRESS, VL2_ADDRESS, VL3_ADDRESS, VL4_ADDRESS, VL5_ADDRESS};
Adafruit_VL6180X *sensors[5]    = {&vl1, &vl2, &vl3, &vl4, &vl5};
extern bool EParkingSlots[4];
extern components componentInit;
extern int empty;

void Task_ParkingSlotCheck(void* param)
{
    while(1)
    {
        #ifdef DEBUG
        uint32_t time2 ;
        uint32_t time = millis();
        #endif
        for (int i = 0; i < 4; i++)
        {
            if(sensors[i]->readRange() < TRIGGER_DISTANCE)
            {
                if(EParkingSlots[i] != true)
                {
                    empty--;
                    EParkingSlots[i] = true;
                }
            }else
            {
                if(EParkingSlots[i] != false)
                {
                    EParkingSlots[i] = false;
                    empty++;
                }
            } 
        }
        #ifdef DEBUG
        time2 = millis() - time;
        Serial.println(time2);
        #endif

        vTaskDelay(pdMS_TO_TICKS(300));
    }

}

void Task_ExitRamp(void* param)
{
    uint8_t distance;
    while(1)
    {
        distance = sensors[4]->readRange();
        if(distance < TRIGGER_DISTANCE && distance != 0)
        {
            vTaskResume(xTaskExitRamp);
            #ifdef DEBUG
                Serial.println(distance);
            #endif
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void VL_Setup()
{ 
    if(!Wire.begin())
    {    
        for(int i = 0; i < 5; i++){
            componentInit.distance[i] = {false};
        }
        return;
    }
    Wire.setClock(I2C_SPEED);
  
    pinMode(SHDN_VL1, OUTPUT);
    pinMode(SHDN_VL2, OUTPUT);
    pinMode(SHDN_VL3, OUTPUT);
    pinMode(SHDN_VL4, OUTPUT);
    pinMode(SHDN_VL5, OUTPUT);

    digitalWrite(SHDN_VL1, LOW);
    digitalWrite(SHDN_VL2, LOW);
    digitalWrite(SHDN_VL3, LOW);
    digitalWrite(SHDN_VL4, LOW);
    digitalWrite(SHDN_VL5, LOW);

    setupAddress();
}

void setupAddress()
{
    for(int i = 0; i < 5; i++)
    {
        digitalWrite(SHDN[i], HIGH);
        delay(10);

        if (!sensors[i]->begin()) {
            #ifdef DEBUG
            Serial.printf("Failed to boot %d. VL6180X\n", i+1);
            #endif
            componentInit.distance[i] = false;
        }
        else{
            sensors[i]->setAddress(VL_Address[i]);
            #ifdef DEBUG
            Serial.printf("VL6180X sensor: %d is initialized on address: 0x%X!\n", i+1, sensors[i]->getAddress());
            #endif
            componentInit.distance[i] = true;
        }
        delay(10);
    }
    #ifdef DEBUG
    check();
    #endif
}

#ifdef DEBUG
void check()
{
    Serial.println("Scanning I2C bus...");
    byte error, address;
    int devices = 0;

    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
        if (address<16)
            Serial.print("0");
        Serial.print(address,HEX);
        Serial.println(" !");
        devices++;
        }    
    }
    Serial.println("Done");
    if (devices == 0)
        Serial.println("No I2C devices found");

    delay(1000); // Adjust delay as needed
}
#endif
