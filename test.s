					AREA	TEST_LCD_C, CODE, READONLY, ALIGN=2
					THUMB
						
					EXTERN	SET_CURSOR
					EXTERN	DISP_CHAR
					EXTERN	SEND_BYTE_SSI
					EXPORT	TEST_LCD
							
TEST_LCD			PROC
					PUSH	{LR}
					
					MOV		R0,#0
					MOV		R1,#0
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					ADD		R0,#1
					ADD		R1,#5
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					ADD		R0,#1
					ADD		R1,#5
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					ADD		R0,#1
					ADD		R1,#5
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					ADD		R0,#1
					ADD		R1,#5
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					ADD		R0,#1
					ADD		R1,#5
					BL		SET_CURSOR
					MOV		R5,#0x23
					BL		DISP_CHAR
					
					POP		{LR}
					BX		LR
					ENDP
						
					ALIGN
					END