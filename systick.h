#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

#define SYSTICK_COUNT 79999

void initSystick(void);

void SysTick_Handler(void);
