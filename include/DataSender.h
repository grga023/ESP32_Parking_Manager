#ifndef DATA_SENDER_H
#define DATA_SENDER_H

#include <Arduino.h>
#include <ArduinoJson.h> // Include ArduinoJson library for JSON handling

// extern void sendDataToVercel(const components &data);
extern void sendEParkingSlotsToVercel(const bool slots[4]);

#endif // DATA_SENDER_H
