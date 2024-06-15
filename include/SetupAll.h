#pragma once

#include <string>
#include <cstdint>

// #define DEBUG
#define EVERY_TIME_INIT

#define I2C_SPEED 400000 

#define WIFI_SSID     "Grgur"
#define WIFI_PASWORD  "192837645"

#define ENTRY_RAMP_PIN 15 //GPIO15, servo1
#define EXIT_RAMP_PIN 2 //GPIO2, servo2

#define PWM_FREQUENCY 50 
#define PWM_RESOLUTION 10 

#define SDA_PIN 21 //GPIO21, I2C data
#define SCL_PIN 22 //GPIO22, I2C clock

#define MAX 10

#define SHDN_VL1 32
#define SHDN_VL2 33
#define SHDN_VL3 25
#define SHDN_VL4 26
#define SHDN_VL5 27

#define TRIGGER_DISTANCE 60

#define VL1_ADDRESS 0x2A
#define VL2_ADDRESS 0x2B
#define VL3_ADDRESS 0x2C
#define VL4_ADDRESS 0x2D
#define VL5_ADDRESS 0x2E

#define OPEN_RAMP_DELAY 3000


typedef uint8_t id[4];

typedef struct
{
  id personId;
  std::string name;
} person;

typedef person persons[MAX];

typedef bool distanceSensors[5];

typedef struct
{
  bool rfid;
  bool display;
  distanceSensors distance;
} components;

