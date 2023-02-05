;LABEL 	DIRECTIVE VALUE COMMENT 
		AREA subroutines, READONLY, CODE, ALIGN=2
		THUMB
		EXPORT delay01ms
			
delay01ms 	PROC 
			PUSH {R0}
			
			LDR r0,=312		;0.1 ms delay
			
			
AGAIN	 	NOP
			NOP
			SUBS R0,#1 		;1 cycle
			BNE AGAIN 			;2 cycle
			POP {R0} 
			BX LR 
			ENDP
				
			ALIGN
			END