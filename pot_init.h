#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

void initPot()
{
	SYSCTL->RCGCADC |= 1; // enable ADC0
	SYSCTL->RCGCGPIO |= 16; // enable port e
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	// PE3
	GPIOE->AFSEL |= 8;
	GPIOE->DIR |= 8;
	GPIOE->AMSEL |= 8;
	GPIOE->DEN &= 0;

	// ADC0
	ADC0->ACTSS &= 7; // disable before configuring
	ADC0->EMUX &= 0x0FFF;
	ADC0->SSCTL3 |= 6;
	ADC0->PC = 0x1;
	ADC0->ACTSS |= 8; // enable
	
	return;
}
	
