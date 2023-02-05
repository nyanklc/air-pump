#ifndef INCLUDE_TM
	#include "TM4C123GH6PM.h"
	#define INCLUDE_TM
#endif

void I2C3_init(void);
static int I2C_wait_till_done(void);
char I2C3_Write(int slave_address, char slave_memory_address, int bytes_count, char* data);
char I2C3_Read(int slave_address, char slave_memory_address, int bytes_count, char* data);


void I2C3_init(void){
	volatile int *NVIC_EN2 = (volatile int*) 0xE000E108;
	volatile int *NVIC_PRI17 = (volatile int*) 0xE000E444;
	
	SYSCTL-> RCGCI2C	|= 0x08;		//active clock i2c3
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	SYSCTL->RCGCGPIO |= 0x08; 	//active clock port D
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
	GPIOD->DEN		|= 0x03;			//PD0 and PD1 digital enable
	GPIOD->DIR		|=0x03;				//output enable
	GPIOD->AMSEL	&= ~0x03;			//PD0 and PD1 analog disable
	GPIOD->AFSEL	|= 0x03;			//enable alternate functions selected
	GPIOD->PCTL &= ~0x000000FF; 
	GPIOD->PCTL		|= 0x00000033; //select I2CSCL,I2CSDA
	GPIOD->ODR		|= 0x02;			//PD1 is SDA and open drain enable
	
	I2C3->MCR 		=0x0010;		//initialize I2C master
	/* Configure I2C clock frequency
	(1 + TIME_PERIOD ) = SYS_CLK /(2*
	( SCL_LP + SCL_HP ) * I2C_CLK_Freq )
	TIME_PERIOD = 16 ,000 ,000/(2(6+4) *100000) - 1 = 7 */
	I2C3->MTPR		=0x07;					//sysclk 16 MHz
	
	//I2C3 interrupt 37
	//Interrupt 36-39 are handled by NVIC register PRI17
	//Interrupt 37 is controlled by bits 15:13 of PRI17
	*NVIC_PRI17 &=0xFFFF0FFF; //Clear interrupt 37 priority
	*NVIC_PRI17 |=0x00004000; //Set interrupt 37 priority to 2
	
	//NVIC has to be enabled
	//Interrupts 69 are handled by NVIC register EN1
	//Interrupt 69 is controlled by bit 5
	*NVIC_EN2 |=0x00000020;
}

static int I2C_wait_till_done(void){
    while(I2C3->MCS & 1);   		// wait until I2C master is not busy 
    return I2C3->MCS & 0xE; 		// return I2C error code, 0 if no error
}

char I2C3_Write(int slave_address, char slave_memory_address, int bytes_count, char* data)
{   
    char error;
    if (bytes_count <= 0)
        return -1;                  /* no write was performed */
    /* send slave address and starting address */
    I2C3->MSA = slave_address << 1;
    I2C3->MDR = slave_memory_address;
    I2C3->MCS = 3;                  /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done();   /* wait until write is complete */
    if (error) return error;

    /* send data one byte at a time */
    while (bytes_count > 1)
    {
        I2C3->MDR = *data++;             /* write the next byte */
        I2C3->MCS = 1;                   /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
    }
    
    /* send last byte and a STOP */
    I2C3->MDR = *data++;                 /* write the last byte */
    I2C3->MCS = 5;                       /* -data-ACK-P */
    error = I2C_wait_till_done();
    while(I2C3->MCS & 0x40);             /* wait until bus is not busy */
    if (error) return error;
    return 0;       /* no error */
}

char I2C3_Read(int slave_address, char slave_memory_address, int bytes_count, char* data)
{
    char error;
    
    if (bytes_count <= 0)
        return -1;         /* no read was performed */

    /* send slave address and starting address */
    I2C3->MSA = slave_address << 1;
    I2C3->MDR = slave_memory_address;
    I2C3->MCS = 3;       /* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();
    if (error)
        return error;

    /* to change bus from write to read, send restart with slave addr */
    I2C3->MSA = (slave_address << 1) + 1;   /* restart: -R-(saddr+r)-ACK */

    if (bytes_count == 1)             /* if last byte, don't ack */
        I2C3->MCS = 7;              /* -data-NACK-P */
    else                            /* else ack */
        I2C3->MCS = 0xB;            /* -data-ACK- */
    error = I2C_wait_till_done();
    if (error) return error;

    *data++ = I2C3->MDR;            /* store the data received */

    if (--bytes_count == 0)           /* if single byte read, done */
    {
        while(I2C3->MCS & 0x40);    /* wait until bus is not busy */
        return 0;       /* no error */
    }
 
    /* read the rest of the bytes */
    while (bytes_count > 1)
    {
        I2C3->MCS = 9;              /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
        *data++ = I2C3->MDR;        /* store data received */
    }

    I2C3->MCS = 5;                  /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C3->MDR;              /* store data received */
    while(I2C3->MCS & 0x40);        /* wait until bus is not busy */
    
    return 0;       /* no error */
}
