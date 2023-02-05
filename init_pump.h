#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

void initPumps(void)
{
	// port a configuration
	// other registers were already configured
	// by lcd
	// PC4, PC5
	GPIOA->DIR |= 0x3;
	GPIOA->DEN |= 0x3;
	// also configure PF2, PF3, PF1
	SYSCTL->RCGCGPIO |= 32;
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	GPIOF->AFSEL &= 0;
	GPIOF->AMSEL &= 0;
	GPIOF->PCTL &= 0;
	
	return;
}
