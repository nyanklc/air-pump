; RST		PA7
; DC		PA6 (D = 1, C = 0)
; CE		PA3 spi
; DIN		PA5 spi
; CLK		PA2 spi

SYSCTL_RCGCGPIO  	EQU 	0x400FE608
GPIO_PORTA_DATA		EQU		0x400043FC
GPIO_PORTA_DIR   	EQU 	0x40004400
GPIO_PORTA_AFSEL 	EQU 	0x40004420
GPIO_PORTA_DEN   	EQU 	0x4000451C
GPIO_PORTA_PCTL  	EQU 	0x4000452C
GPIO_PORTA_AMSEL 	EQU 	0x40004528
	
SYSCTL_RCGCSSI		EQU		0x400FE61C
SSI0_CR0			EQU		0x40008000
SSI0_CR1			EQU		0x40008004
SSI0_DR				EQU		0x40008008
SSI0_SR				EQU		0x4000800C
SSI0_CPSR			EQU		0x40008010
SSI0_CC				EQU		0x40008FC8
PRSSI				EQU 	0x400FEA1C
	

				AREA init_lcd,	CODE,	READONLY,	ALIGN=2
				THUMB
				EXPORT INIT_LCD
				EXTERN SEND_BYTE_SSI
				
INIT_LCD			PROC
					PUSH {LR}

					; SETUP GPIO
					; enable port A clock
					LDR 	R1, =SYSCTL_RCGCGPIO
					LDR 	R0, [R1]             
					ORR 	R0, #0x01				
					STR 	R0, [R1]     
					; wait until clock stabilizes
					NOP
					NOP
					NOP
					NOP
					NOP
					NOP
					NOP
					; DIR
					LDR		R1,=GPIO_PORTA_DIR
					LDR		R0, [R1]
					ORR 	R0, #0xEC
					STR		R0,[R1]
					; PA2, PA3, PA5 -> alternate function
					LDR		R1,=GPIO_PORTA_AFSEL
					LDR		R0, [R1]
					ORR	 	R0, #0x2C
					STR		R0,[R1]
					; DEN
					LDR		R1,=GPIO_PORTA_DEN	
					LDR		R0, [R1]
					ORR		R0, #0XEC
					STR		R0,[R1]
					; PA2, PA3, PA5 -> SSI
					LDR		R1,=GPIO_PORTA_PCTL
					LDR 	R0, =0x00202200		
					STR		R0,[R1]
					; disable analog
					LDR		R1,=GPIO_PORTA_AMSEL
					LDR		R0, [R1]
					BIC 	R0, #0xEC
					STR		R0,[R1]
										
					; SETUP SSI
					LDR 	R1,=SYSCTL_RCGCSSI
					LDR 	R0,[R1]                   
					ORR 	R0, #0x01 ; ssi0
					STR 	R0,[R1]            
					
					; wait until clock stabilizes
WAIT				LDR 	R1, =PRSSI
					LDRB 	R0, [R1]
					ANDS 	R0, #0x01
					BEQ		WAIT
					
					; disable ssi0
					LDR		R1,=SSI0_CR1
					MOV		R0, #0x00
					STR		R0,[R1]
					; 1MHZ BAUD, SYSCLOCK, CPSDVSR = 8, SCR = 1
					LDR		R1,=SSI0_CC
					MOV		R0,#0x05				; PIOSC
					STR		R0,[R1]	
					LDR		R1,=SSI0_CR0		
					LDR		R0,[R1]
					ORR		R0, #0x0100				; SCR = 1 (1MHz)
					STR		R0,[R1]
					LDR		R1,=SSI0_CPSR		
					MOV 	R0, #0x08				; CPSDVSR 8
					STR		R0,[R1]
					; configure freescale and 8 bit
					LDR		R1,=SSI0_CR0		
					LDR		R0,[R1]					
					BIC		R0, #0x3F				; FREESCALE 	
					ORR		R0, #0x07				; 8BIT
					STR		R0,[R1]
					; enable ssi0
					LDR		R1,=SSI0_CR1			; ENABLE SSI
					LDR		R0,[R1]
					ORR 	R0, #0x02
					STR		R0,[R1]
					
					; SETUP LCD
					LDR		R1, =GPIO_PORTA_DATA
					; set reset low for 100ms
					LDR		R0,[R1]
					BIC		R0, #0x80
					STR		R0,[R1]
					LDR		R0, =150000
WAIT_100ms			NOP
					NOP
					NOP
					NOP
					SUBS	R0, #1
					BNE		WAIT_100ms
					; set reset high
					LDR		R0,[R1]
					ORR		R0, #0x80
					STR		R0,[R1]
					
					
					; switch to command mode
					LDR		R1, =GPIO_PORTA_DATA
					LDR		R0,[R1]
					BIC		R0, #0x40
					STR		R0,[R1]
					
					MOV 	R0, #0x21		; LCD Extended Commands.
					BL 		SEND_BYTE_SSI	
					MOV 	R0, #0xB0		; Set LCD Vop
					BL 		SEND_BYTE_SSI
					MOV 	R0, #0x04		; Set Temp coefficent.
					BL 		SEND_BYTE_SSI
					MOV 	R0, #0x20 		; Voltage Bias.
					BL 		SEND_BYTE_SSI
					MOV 	R0, #0x20		; LCD Basic Commands.
					BL 		SEND_BYTE_SSI
					MOV 	R0, #0x0C		; LCD Normal Mode 
					BL 		SEND_BYTE_SSI
					
					; CLEAR SCREEN
CLRBSY				LDR		R1,=SSI0_SR				; CHECK BUSY
					LDR		R0,[R1]
					ANDS	R0,R0,#0x10
					BNE		CLRBSY
					LDR		R1,=GPIO_PORTA_DATA		
					LDR		R0,[R1]
					ORR		R0,#0x40				; DATA MODE
					STR		R0,[R1]	
					MOV		R3,#504					; 48*84/8					
					MOV		R0,#0x00
					
CLRLOOP				BL		SEND_BYTE_SSI
					SUBS	R3,#1
					BNE		CLRLOOP
					
CLREND				LDR		R1,=SSI0_SR				
					LDR		R0,[R1]
					ANDS	R0,R0,#0x10				;CHECK BUSY
					BNE		CLREND
					
					POP 	{LR}
					BX 		LR
					ENDP

					ALIGN
					END
					
					
					
					