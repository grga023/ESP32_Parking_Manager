#pragma once
#include "esp_freertos_hooks.h"
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Arduino.h"

#define uint64_t long

extern esp_err_t perfmon_start();