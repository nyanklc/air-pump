#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

#include "i2c.h"

#define SLAVE_ADDR   0x76                 /* 1110110 */

/**
 * BMP280 registers
 */
#define BMP280_REG_TEMP_XLSB   0xFC /* bits: 7-4 */
#define BMP280_REG_TEMP_LSB    0xFB
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_TEMP        (BMP280_REG_TEMP_MSB)
#define BMP280_REG_PRESS_XLSB  0xF9 /* bits: 7-4 */
#define BMP280_REG_PRESS_LSB   0xF8
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_PRESSURE    (BMP280_REG_PRESS_MSB)
#define BMP280_REG_CONFIG      0xF5 /* bits: 7-5 t_sb; 4-2 filter; 0 spi3w_en */
#define BMP280_REG_CTRL        0xF4 /* bits: 7-5 osrs_t; 4-2 osrs_p; 1-0 mode */
#define BMP280_REG_STATUS      0xF3 /* bits: 3 measuring; 0 im_update */
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_ID          0xD0
#define BMP280_REG_CALIB       0x88


#define BMP280_DIG_T1_LSB_ADDR               0x88
#define BMP280_DIG_T1_MSB_ADDR               0x89
#define BMP280_DIG_T2_LSB_ADDR               0x8A
#define BMP280_DIG_T2_MSB_ADDR               0x8B
#define BMP280_DIG_T3_LSB_ADDR               0x8C
#define BMP280_DIG_T3_MSB_ADDR               0x8D
#define BMP280_DIG_P1_LSB_ADDR               0x8E
#define BMP280_DIG_P1_MSB_ADDR               0x8F
#define BMP280_DIG_P2_LSB_ADDR               0x90
#define BMP280_DIG_P2_MSB_ADDR               0x91
#define BMP280_DIG_P3_LSB_ADDR               0x92
#define BMP280_DIG_P3_MSB_ADDR               0x93
#define BMP280_DIG_P4_LSB_ADDR               0x94
#define BMP280_DIG_P4_MSB_ADDR               0x95
#define BMP280_DIG_P5_LSB_ADDR               0x96
#define BMP280_DIG_P5_MSB_ADDR               0x97
#define BMP280_DIG_P6_LSB_ADDR               0x98
#define BMP280_DIG_P6_MSB_ADDR               0x99
#define BMP280_DIG_P7_LSB_ADDR               0x9A
#define BMP280_DIG_P7_MSB_ADDR               0x9B
#define BMP280_DIG_P8_LSB_ADDR               0x9C
#define BMP280_DIG_P8_MSB_ADDR               0x9D
#define BMP280_DIG_P9_LSB_ADDR               0x9E
#define BMP280_DIG_P9_MSB_ADDR               0x9F


void bmp280_reset(void);
void bmp280_init_temp(void);
void bmp280_init_press(void);
void bmp280_init(void);
uint32_t bmp280_tfine(void);

void bmp280_reset(void){
	char BMP280_RESET_VALUE[1];    
	*BMP280_RESET_VALUE = 0xB6;
	I2C3_Write(SLAVE_ADDR,BMP280_REG_RESET,1,BMP280_RESET_VALUE);
}

void bmp280_init_temp(void){
	char BMP280_CTRL_VALUE[1];
	char BMP280_CONFIG_VALUE[1];
	
	*BMP280_CTRL_VALUE = 0x23;
	*BMP280_CONFIG_VALUE = 0x00;
	
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CTRL,1,BMP280_CTRL_VALUE); //normal mode, oversamplingx1
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CONFIG,1,BMP280_CONFIG_VALUE);		//t_sb 0.5ms
}

void bmp280_init_press(void){
	char BMP280_CTRL_VALUE[1];
	char BMP280_CONFIG_VALUE[1];
	
	*BMP280_CTRL_VALUE = 0x07;
	*BMP280_CONFIG_VALUE = 0x00;
	
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CTRL,1,BMP280_CTRL_VALUE); //normal mode, oversamplingx1
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CONFIG,1,BMP280_CONFIG_VALUE);		//t_sb 0.5ms
}

void bmp280_init(void){
	char BMP280_CTRL_VALUE[1];
	char BMP280_CONFIG_VALUE[1];
	
	*BMP280_CTRL_VALUE = 0x27;
	*BMP280_CONFIG_VALUE = 0x00;
	
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CTRL,1,BMP280_CTRL_VALUE); //normal mode, oversamplingx1
	I2C3_Write(SLAVE_ADDR,BMP280_REG_CONFIG,1,BMP280_CONFIG_VALUE);		//t_sb 0.5ms
}

uint32_t bmp280_tfine(void){
	char temp_msb_arr[256],temp_lsb_arr[256];
	volatile int i,temp_msb,temp_lsb, temp;
	char trimming_temp_values[6];
	volatile unsigned short dig_T1;
	volatile short dig_T2, dig_T3;
	volatile float var1, var2, T;
	uint32_t t_fine;
	
	bmp280_init_temp();
	
	I2C3_Read(SLAVE_ADDR,BMP280_DIG_T1_LSB_ADDR,6,trimming_temp_values);	
	dig_T1 = (trimming_temp_values[0]) + (trimming_temp_values[1])*16;
	dig_T2 = trimming_temp_values[2] + trimming_temp_values[3]*16;	
	dig_T3 = trimming_temp_values[4] + trimming_temp_values[5]*16;
	
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
	
	return t_fine;
}
