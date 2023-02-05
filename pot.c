#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

volatile int pot_sample_ = 0;

int handlePot(void)
{
	// tell the adc to start sampling (sequencer 3)
	ADC0->PSSI |= 8;
	// wait for the sampling to be completed
	while (1)
	{
		if ((ADC0->RIS & 8) == 8) // flag is detected
		{
			// turn sampling off
			ADC0->PSSI &= 7;
			// clear the interrupt flag
			ADC0->ISC |= 8;

			pot_sample_ = ADC0->SSFIFO3;
			break;
		}
	}
	return pot_sample_;
}

int convertPotValue(int pot, int min, int max)
{
	// max value of pot = 0xFFF
	// min value of pot = 0x000
	
	float pot_range = 0xFFF;
	float range_range = (float)max - (float)min;
	
	float percentage = (float)pot / pot_range;
	
	return (int) (percentage * range_range + (float)min);
}
