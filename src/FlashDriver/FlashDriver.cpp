#include "FlashDriver.h"


void writeToFlash_4K(const person* persons, size_t length) {
    // Calculate the size of the data to write
    size_t dataSize = length * sizeof(person);
    
    // Erase the sector before writing
    esp_err_t err = spi_flash_erase_range(FLASH_ADDRESS, 0x1000);
    if (err != ESP_OK) {
        Serial.println("Error erasing flash!");
        return;
    }

    // Write the persons array to the flash
    err = spi_flash_write(FLASH_ADDRESS, persons, dataSize);
    if (err != ESP_OK) {
        Serial.println("Error writing to flash!");
    }
}

void readPersonsFromFlash_4K(persons persons) {
    size_t dataSize = MAX * sizeof(person);

    esp_err_t err = spi_flash_read(FLASH_ADDRESS, persons, dataSize);
    if (err != ESP_OK) {
        Serial.println("Error reading from flash!");
    }
}
