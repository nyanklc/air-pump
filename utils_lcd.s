				AREA utils_lcd,	CODE,	READONLY,	ALIGN=2
				THUMB
			
				EXPORT		SET_CURSOR
				EXPORT		DISP_CHAR
				EXPORT		SEND_BYTE_SSI
				EXPORT		DRAW_TEXT
				EXTERN		ASCII
					
GPIO_PORTA_DATA	    EQU 0x400043FC
SSI0_SR 			EQU 0x4000800C ; for busy and fill status
SSI0_DR 			EQU 0x40008008 ; for writing data


; DISPLAY CHARACTER IN R5
DISP_CHAR			PROC
					PUSH	{R0-R4,LR}
					LDR		R1,=GPIO_PORTA_DATA		; DATA MODE
					LDR		R0,[R1]
					ORR		R0,#0x40
					STR		R0,[R1]
					LDR		R1,=ASCII
					SUB		R2,R5,#0x20				; OFFSET
					MOV		R3,#0x05
					MUL		R2,R2,R3				; EACH CHAR IS 5 BYTE
					ADD		R1,R1,R2				;ADD R1 TO CHAR VALUE

					MOV		R0,#0x05				; EACH CHAR IS 5 BYTE
LOOP				LDRB	R5,[R1],#1
					PUSH	{R0}
					MOV		R0,R5
					BL		SEND_BYTE_SSI
					POP		{R0}
					SUBS	R0,R0,#1
					BNE		LOOP
					
					MOV		R5,#0X00				; SPACE AFTER
					PUSH	{R0}
					MOV		R0,R5
					BL		SEND_BYTE_SSI
					POP		{R0}				
					
CHECK_BUSYX			LDR		R1,=SSI0_SR			
					LDR		R0,[R1]
					ANDS	R0,R0,#0x10				; CHECK BUSY
					BNE		CHECK_BUSYX
					
					POP		{R0-R4,LR}
					BX		LR
					ENDP
						
					;set the cursor position, R0-x, R1-y
SET_CURSOR			PROC
					PUSH	{R0-R5,LR}
					
					; switch r0 and r1
					; because I decided to use R0 for row
					; and R1 for column
					; and I'm lazy
					PUSH	{R2}
					MOV		R2,R0
					MOV		R0,R1
					MOV		R1,R2
					POP		{R2}

					PUSH	{R0-R2}
					LDR		R1,=GPIO_PORTA_DATA ;command mode
					MOV		R2,R1
					LDR		R0,[R2]
					BIC		R0,#0x40
					STR		R0,[R1]
					POP		{R0-R2}
					
					MOV		R5,R1 ;y
					ORR		R5,#0x40
					PUSH	{R0}
					MOV		R0,R5
					BL		SEND_BYTE_SSI
					POP		{R0}
					MOV		R5,R0 ;x
					ORR		R5,#0x80
					PUSH	{R0}
					MOV		R0,R5
					BL		SEND_BYTE_SSI
					POP		{R0}
					
WAITUD				LDR		R1,=SSI0_SR ;wait until done
					LDR		R0,[R1]
					ANDS	R0,R0,#0x10
					BNE		WAITUD
					
					LDR		R1,=GPIO_PORTA_DATA ;normal mode
					LDR		R0,[R1]
					ORR		R0,#0x40
					STR		R0,[R1]
					POP		{R0-R5,LR}
					BX		LR
					ENDP
						
SEND_BYTE_SSI	 	PROC
					PUSH 	{R0, R1, R2}
					
					; CE 0
					LDR		R1, =GPIO_PORTA_DATA
					LDR 	R2, [R1]
					BIC 	R2, #0x08
					STR 	R2, [R1]
					
					; wait until fifo is not full
					LDR 	R1, =SSI0_SR
CHECK_FULL			LDR 	R2, [R1]
					ANDS	R2, #0x02
					BEQ		CHECK_FULL
					
					; wait until ssi is not busy
CHECK_BUSY			LDR 	R2, [R1]
					ANDS 	R2, #0x10
					BNE 	CHECK_BUSY
					
					; store in fifo
					LDR 	R1, =SSI0_DR
					STRB 	R0, [R1]
					
					; wait until ssi is not busy (wait until sent)
					LDR 	R1, =SSI0_SR
CHECK_BUSY2			LDR 	R2, [R1]
					ANDS 	R2, #0x10
					BNE 	CHECK_BUSY2

					; CE 1
					LDR 	R1, =GPIO_PORTA_DATA
					LDR 	R0, [R1]
					ORR 	R0, #0x08
					STRB 	R0, [R1]
					
					POP 	{R0, R1, R2}
					BX 		LR
					ENDP
						
DRAW_TEXT			PROC
					PUSH	{R0,R1,R5,LR}
					
					; CURR
					MOV		R0,#0
					MOV		R1,#0
					BL		SET_CURSOR
					MOV		R5,#0x43
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x55
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x52
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x52
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x3A
					BL		DISP_CHAR
					
					; MIN
					MOV		R0,#4
					MOV		R1,#0
					BL		SET_CURSOR
					MOV		R5,#0x4D
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x41
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x58
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x3A
					BL		DISP_CHAR
					
					; MAX
					MOV		R0,#2
					MOV		R1,#0
					BL		SET_CURSOR
					MOV		R5,#0x4D
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x49
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x4E
					BL		DISP_CHAR
					
					ADD		R1,#6
					BL		SET_CURSOR
					MOV		R5,#0x3A
					BL		DISP_CHAR
					
					POP		{R0,R1,R5,LR}
					BX		LR
					ENDP
						
					ALIGN
					END