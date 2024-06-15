#ifndef SERIAL_MAIN
#define SERIAL_MAIN

#include <Arduino.h>


void SerialCommand();
extern void SerialHandler(bool *EParkingSlots);
extern void Task_SerialState(void* param);

#endif