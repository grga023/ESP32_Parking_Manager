#include "Display_main.h"
#include <Wire.h>
#include <iostream>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "SetupAll.h"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
extern int empty;
extern components componentInit;
int lastState;

void setupDisplay()
{
  if (!display.begin(0x3C, true)) {
    componentInit.display = false;
    return;
  }
  componentInit.display = true;
  display.clearDisplay();
  display.setRotation(3); //rotation of display - horizontal
  // text display tests
  display.setTextSize(3);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(5,10);
  display.println("SMART");
  display.println("PARKING");
  display.display(); // actually display all of the above
  delay(2000);
  display.clearDisplay();
}

void WriteOnDisplay(int textSize, int cursorX, int cursorY, const std::string& text, bool clear)
{
  if (clear)
  {
    display.clearDisplay(); 
  }
  display.setTextSize(textSize);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(cursorX, cursorY);
  display.println(text.c_str()); 
}

void displayContent()
{
  display.display(); 
}

void Task_DisplayState(void* param)
{
  std::string emptySlots = "";
  lastState = 0;
  while (1)
  {
    switch (empty)
    {
      case 0:
        WriteOnDisplay(3, 0, 0, "PARKING FULL", true);
        displayContent();
        break;
      
      default:
        if(empty != lastState)
        {
          WriteOnDisplay(2, 5, 0, "FREE SPOTS", true);
          emptySlots = std::to_string(empty);
          WriteOnDisplay(4, 25, 25, emptySlots, false);
          WriteOnDisplay(4, 48, 25, "/4", false);
          display.display(); 
          lastState = empty;
        }
        break;
    }

  }
  vTaskDelay(pdMS_TO_TICKS(500));
}