#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

#include "init_system.h"

#ifndef INCLUDE_SV
	#include "svalues.h"
	#define INCLUDE_SV
#endif

#include "pump.h"

#include "pot.h"

#define PRESSURE_RANGE 20
#define PRESSURE_MIN_MIN 60
#define PRESSURE_MIN_MAX 100

extern SHOW_VALS(Values*);
extern TEST_LCD(void);

static int test = 100;

int main(void)
{
	initSystem();
	
	while (1)
	{
		// read range from pot
		setRange(
			getValues(),
			convertPotValue(handlePot(), PRESSURE_MIN_MIN, PRESSURE_MIN_MAX),
			PRESSURE_RANGE);
		
		// wait for 256 samples
		if (getValues()->current_ready == 0)
		{
			SysTick_Handler(); // systick is off currently, we're collecting samples manually
			continue;
		}
		getValues()->current_ready = 0;
		
//		// DENEME
//		test+=2;
//		getValues()->current = test;
//		getValues()->current_digit_count = getDigitCount(getValues()->current);
//		if (test > 160)
//			break;
//		// DENEME END
		
		drivePumps(*getValues());
		
		// TODO: output every 1 second
		SHOW_VALS(getValues());
	}
	
	
	while (1);
}
