#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

#ifndef INCLUDE_SV
	#include "svalues.h"
	#define INCLUDE_SV
#endif

void drivePumps(Values vals);

void driveInflating(void);

void driveDeflating(void);

void stopPumps(void);