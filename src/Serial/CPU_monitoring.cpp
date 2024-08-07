#include "CPU_monitoring.h"

static uint64_t idle0Calls = 0;
static uint64_t idle1Calls = 0;

#if defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_240)
static const uint64_t MaxIdleCalls = 1855000;
#elif defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_160)
static const uint64_t MaxIdleCalls = 1233100;
#else
#error "Unsupported CPU frequency"
#endif

int cpu1;

static bool idle_task_1()
{
	idle1Calls += 1;
	return false;
}

static void perfmon_task(void *args)
{
	while (1)
	{

		float idle1 = idle1Calls;
		idle1Calls = 0;

		cpu1 = 100.f - idle1 / MaxIdleCalls * 100.f;

		// Serial.printf( "Core 1 at %d%%", cpu1);
		// TODO configurable delay
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	vTaskDelete(NULL);
}

esp_err_t perfmon_start()
{
	ESP_ERROR_CHECK(esp_register_freertos_idle_hook_for_cpu(idle_task_1, 1));
	// TODO calculate optimal stack size
	xTaskCreate(perfmon_task, "perfmon", 2048, NULL, 1, NULL);
	return ESP_OK;
}