#include "pump.h"

void drivePumps(Values vals)
{	
	if (vals.current < vals.min_range)
	{
		driveInflating();
	}
	else if (vals.current > vals.max_range)
	{
		driveDeflating();
	}
	else
	{
		stopPumps();
	}
}

void red(int val)
{
	if (val)
	{
		GPIOF->DATA |= 1 << 1;
	}
	else	
	{
		GPIOF->DATA &= ~(1 << 1);
	}
	
	return;
}

void blue(int val)
{
	if (val)
	{
		GPIOF->DATA |= 1 << 2;
	}
	else	
	{
		GPIOF->DATA &= ~(1 << 2);
	}
	
	return;
}

void green(int val)
{
	if (val)
	{
		GPIOF->DATA |= 1 << 3;
	}
	else	
	{
		GPIOF->DATA &= ~(1 << 3);
	}
	
	return;
}

void driveInflating(void)
{
	GPIOA->DATA |= 1; // PA0 high
	GPIOA->DATA &= 0xFFFFFFFD; // PA1 low
	
	// also drive LEDs
	red(1);
	green(0);
	blue(0);
}

void driveDeflating(void)
{
	GPIOA->DATA |= 2; // PA1 high
	GPIOA->DATA &= 0xFFFFFFFE; // PA0 low
	
	red(0);
	green(0);
	blue(1);
}

void stopPumps(void)
{
	GPIOA->DATA &= 0xFFFFFFFC;
	
	red(0);
	green(1);
	blue(0);
}
