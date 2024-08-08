#pragma once 
#include "SetupAll.h"
#include "Arduino.h"
#include "esp_partition.h"
#include "esp_spi_flash.h"

extern void readPersonsFromFlash_4K(persons persons);
extern void writePersonsToFlash_4K(const person* persons, size_t length);