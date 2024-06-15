#pragma once
#include "SetupAll.h"

extern void WriteOnDisplay(int textSize, int cursorX, int cursorY, const std::string& text, bool clear);
extern void displayContent();
extern void setupDisplay();
extern void Task_DisplayState(void* param);