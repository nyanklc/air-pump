#include "init_pump.h"
#include "pot_init.h"
#include "systick.h"

#ifndef INCLUDE_SV
	#include "svalues.h"
	#define INCLUDE_SV
#endif


extern INIT_LCD(void);
extern DRAW_TEXT(void);

static void initSystem(void)
{
	// vals
	initValues();

	// lcd
	INIT_LCD();
	DRAW_TEXT();
	
	// pumps
	initPumps();
	
	// pot
	initPot();
	
	// sensor
	I2C3_init();
	bmp280_reset();
	bmp280_init();
	// initSystick(); // call this last
	
	return;
}
