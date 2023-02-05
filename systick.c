#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

#ifndef INCLUDE_SV
	#include "svalues.h"
	#define INCLUDE_SV
#endif

#include "systick.h"

#include "bmp280.h"

	char press_msb_arr[256],press_lsb_arr[256];
	char temp_msb_arr[256],temp_lsb_arr[256];
	// volatile int i, press_lsb, press_msb, press, temp_msb, temp_lsb, temp;
	static int i, press_lsb, press_msb, press, temp_msb, temp_lsb, temp;
	char trimming_press_values[18];
	char trimming_temp_values[6];
	static short dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
	unsigned short dig_P1,dig_T1;

	volatile double var1, var2, T, P;
	volatile uint32_t t_fine;

int getPressure()
{
	P = 0;
	T = 0;
	temp_lsb = 0;
	temp_msb = 0;
	press_lsb = 0;
	press_msb = 0;
	
	I2C3_Read(SLAVE_ADDR,BMP280_DIG_T1_LSB_ADDR,6,trimming_temp_values);	
	dig_T1 = ((unsigned short)trimming_temp_values[0]) + ((unsigned short)trimming_temp_values[1]*256);
	dig_T2 = ((short)trimming_temp_values[2]) + ((short)trimming_temp_values[3]*256);	
	dig_T3 = ((short)trimming_temp_values[4]) + ((short)trimming_temp_values[5]*256);
	
	I2C3_Read(SLAVE_ADDR,BMP280_DIG_P1_LSB_ADDR,18,trimming_press_values);
	dig_P1 = ((unsigned short)trimming_press_values[0]) + ((unsigned short)trimming_press_values[1]*256);
	dig_P2 = ((short)trimming_press_values[2]) + ((short)trimming_press_values[3]*256);
	dig_P3 = ((short)trimming_press_values[4]) + ((short)trimming_press_values[5]*256);
	dig_P4 = ((short)trimming_press_values[6]) + ((short)trimming_press_values[7]*256);
	dig_P5 = ((short)trimming_press_values[8]) + ((short)trimming_press_values[9]*256);
	dig_P6 = ((short)trimming_press_values[10]) + ((short)trimming_press_values[11]*256);
	dig_P7 = ((short)trimming_press_values[12]) + ((short)trimming_press_values[13]*256);
	dig_P8 = ((short)trimming_press_values[14]) + ((short)trimming_press_values[15]*256);
	dig_P9 = ((short)trimming_press_values[16]) + ((short)trimming_press_values[17]*256);
	
	// temperature
	for (i=0; i<256; i++){
		I2C3_Read(SLAVE_ADDR,BMP280_REG_TEMP_MSB,1,temp_msb_arr+i);
		I2C3_Read(SLAVE_ADDR,BMP280_REG_TEMP_LSB,1,temp_lsb_arr+i);
	}
	for(i=0; i<256; i++){
		temp_lsb = temp_lsb + ((int)temp_lsb_arr[i]);
		temp_msb = temp_msb + ((int)temp_msb_arr[i]);
	}
	
	temp_lsb = temp_lsb/256;
	temp =(temp_lsb + temp_msb)*16;	

	var1 = (((double)temp)/16384.0 - ((double)dig_T1)/1024.0)*((double)dig_T2);
	var2 = ((((double)temp)/131072.0 -((double)dig_T1)/8192.0)*(((double)temp)/131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);
	t_fine =((uint32_t)(var1 + var2));
	T = (var1+var2)/5120.0;
	
	// pressure
	for(i=0; i<256; i++){
				I2C3_Read(SLAVE_ADDR,BMP280_REG_PRESS_MSB,1,press_msb_arr+i);
				I2C3_Read(SLAVE_ADDR,BMP280_REG_PRESS_LSB,1,press_lsb_arr+i);
	}	
	for(i=0; i<256; i++){
		press_lsb = press_lsb + ((int)press_lsb_arr[i]);
		press_msb = press_msb + ((int)press_msb_arr[i]);
	}
	
 	press_lsb = press_lsb/256;
	press =(press_lsb + press_msb)*16;	

	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1*var1*((double)dig_P6)/32768.0;
	var2 = var2 + var1*((double)dig_P5)*2.0;
	var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
	var1 = (((double)dig_P3)*var1*var1/524288.0 + ((double)dig_P2)*var1)/524288.0;
	var1 = (1.0 + var1/32768.0)*((double)dig_P1);
	P = 1048576.0 - ((double)press);
	P = (P - (var2/4096.0))*6250.0/var1;
	var1 = ((double)dig_P9)*P*P/2147483648.0;
	var2 = P*((float)dig_P8)/32768.0;
	P = P + (var1 + var2 +((double)dig_P7))/16.0;
	
	return (int)(P);
}

void SysTick_Handler(void)
{
	getValues()->current = getPressure()/1000;
	getValues()->current_digit_count = getDigitCount(getValues()->current);
	getValues()->current_ready = 1;
	
	// ??
	SysTick->VAL = SYSTICK_COUNT;
	
	return;
}

void initSystick(void)
{
	SysTick->LOAD = SYSTICK_COUNT;
	SysTick->CTRL = 7;
	SysTick->VAL = 0;
}

