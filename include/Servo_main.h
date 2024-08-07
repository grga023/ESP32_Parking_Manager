#pragma once

void ExitRamp();
void EnterRamp();
extern void task_entry(void* param);
extern void task_exit(void* param);
extern void setupServo();