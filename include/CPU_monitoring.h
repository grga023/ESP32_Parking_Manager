#pragma once
#include "esp_freertos_hooks.h"
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Arduino.h"

typedef unsigned long long uint64_t;
#define CONFIG_ESP32_DEFAULT_CPU_FREQ_240
extern esp_err_t perfmon_start();