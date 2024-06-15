#pragma once
#include "SetupAll.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

extern void setupRFID();
void Task_RFIDScanner(void* param);
person checkID(uint8_t* uid, person* ljudi);