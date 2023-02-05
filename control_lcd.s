					AREA	CONTROL_LCD, CODE, READONLY, ALIGN=2
					THUMB
						
					EXTERN	SET_CURSOR
					EXTERN	DISP_CHAR
					EXTERN	SEND_BYTE_SSI
					EXPORT	SHOW_VALS
						
; takes in pointer to struct in R0 
SHOW_VALS			PROC
					PUSH	{R0-R5, LR}
					BL		CLR_SCR
					BL		SET_CURR
					ADD		R0,#8
					BL		SET_MIN
					ADD		R0,#8
					BL		SET_MAX
					POP		{R0-R5, LR}
					BX		LR
					ENDP
					ALIGN
				
SET_CURR			PROC
					PUSH	{LR,R9,R6}
					; set cursor (position of the rightmost digit of the number)
					PUSH 	{R0,R1}
					LDR		R0,=0
					LDR		R1,=60
					; hold cursor position in R3 and R4
					MOV		R3,R0
					MOV		R4,R1
					BL		SET_CURSOR
					POP 	{R0,R1}
					; display value
					PUSH	{R0}
					; get digit count in R1
					LDR 	R1,[R0,#4]
					; get value in R0
					LDR		R0,[R0]
					; display digits starting from right
LOOP_CURR			CMP		R1,#0
					BEQ		END_CURR
					MOV		R9,#10
					UDIV	R5,R0,R9
					MOV		R6,R5
					MLS		R5,R5,R9,R0
					MOV		R0,R6
					PUSH	{R5,R1}
					ADD		R5,#0x30
					BL		DISP_CHAR
					POP		{R5,R1}
					PUSH	{R0,R1}
					MOV		R0,R3
					SUB		R4,#6 ; characters are 5 columns in width (+1 space)
					MOV		R1,R4
					BL		SET_CURSOR
					POP		{R0,R1}
					SUBS	R1,#1
					CMP		R1,#0
					BEQ		END_CURR
					B		LOOP_CURR
END_CURR			POP		{R0}
					POP		{LR,R9,R6}
					ENDP
					BX		LR
					ALIGN
						
SET_MIN				PROC
					PUSH	{LR,R9,R6}
					; set cursor (position of the rightmost digit of the number)
					PUSH 	{R0,R1}
					LDR		R0,=2
					LDR		R1,=60
					; hold cursor position in R3 and R4
					MOV		R3,R0
					MOV		R4,R1
					BL		SET_CURSOR
					POP 	{R0,R1}
					; display value
					PUSH	{R0}
					; get digit count in R1
					LDR 	R1,[R0,#4]
					; get value in R0
					LDR		R0,[R0]
					; display digits starting from right
LOOP_MIN			CMP		R1,#0
					BEQ		END_MIN
					MOV		R9,#10
					UDIV	R5,R0,R9
					MOV		R6,R5
					MLS		R5,R5,R9,R0
					MOV		R0,R6
					PUSH	{R5,R1}
					ADD		R5,#0x30
					BL		DISP_CHAR
					POP		{R5,R1}
					PUSH	{R0,R1}
					MOV		R0,R3
					SUB		R4,#6 ; characters are 5 columns in width (1 auto increment)
					MOV		R1,R4
					BL		SET_CURSOR
					POP		{R0,R1}
					SUBS	R1,#1
					CMP		R1,#0
					BEQ		END_MIN
					B		LOOP_MIN
END_MIN				POP		{R0}
					POP		{LR,R9,R6}
					ENDP
					BX		LR
					ALIGN
				
SET_MAX				PROC
					PUSH	{LR,R9,R6}
					; set cursor (position of the rightmost digit of the number)
					PUSH 	{R0,R1}
					LDR		R0,=4
					LDR		R1,=60
					; hold cursor position in R3 and R4
					MOV		R3,R0
					MOV		R4,R1
					BL		SET_CURSOR
					POP 	{R0,R1}
					; display value
					PUSH	{R0}
					; get digit count in R1
					LDR 	R1,[R0,#4]
					; get value in R0
					LDR		R0,[R0]
					; display digits starting from right
LOOP_MAX			CMP		R1,#0
					BEQ		END_MAX
					MOV		R9,#10
					UDIV	R5,R0,R9
					MOV		R6,R5
					MLS		R5,R5,R9,R0
					MOV		R0,R6
					PUSH	{R5,R1}
					ADD		R5,#0x30
					BL		DISP_CHAR
					POP		{R5,R1}
					PUSH	{R0,R1}
					MOV		R0,R3
					SUB		R4,#6 ; characters are 5 columns in width (1 auto increment)
					MOV		R1,R4
					BL		SET_CURSOR
					POP		{R0,R1}
					SUBS	R1,#1
					CMP		R1,#0
					BEQ		END_MAX
					B		LOOP_MAX
END_MAX				POP		{R0}
					POP		{LR,R9,R6}
					ENDP
					BX		LR
					ALIGN
						
CLR_SCR				PROC
					PUSH	{R0-R5,LR}
					MOV		R5,#0x20
					MOV		R2,#0 ;i
					MOV		R3,#60 ;j
CLR_OUTER_LOOP		MOV		R0,R2
CLR_INNER_LOOP		MOV		R1,R3
					SUB		R3,#7
					BL		SET_CURSOR
					PUSH	{R5,R1}
					BL		DISP_CHAR
					POP		{R5,R1}
					CMP		R3,#31
					BHI		CLR_INNER_LOOP
					ADD		R2,#2
					MOV		R3,#60
					CMP		R2,#6
					BNE		CLR_OUTER_LOOP
					POP		{R0-R5,LR}
					ENDP
					BX		LR
					ALIGN
						
					END