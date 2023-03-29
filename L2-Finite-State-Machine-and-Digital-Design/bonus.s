;Template - Defining an Assembly File 
;
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
;
; Modified by: Yaser M. Haddara
; January 28, 2023
; Program for studio 2
; Initial template: implement a combinational lock using FSM
; Goal: implement a sequential lock using FSM
;
; McMaster University, Comp Eng 2DX3, Winter 2023
; Further resources: Valvano textbook, Chs. 3 & 4
;

;ADDRESS SETUP
;Define your I/O Port Addresses Here

SYSCTL_RCGCGPIO_R       EQU		0x400FE608         ;Step 1: GPIO Run Mode Clock Gating Control Register Address

GPIO_PORTN_DIR_R		EQU 	0x40064400  ;GPIO Port N Direction Register address 
GPIO_PORTN_DEN_R        EQU 	0x4006451C  ;GPIO Port N Digital Enable Register address
GPIO_PORTN_DATA_R       EQU 	0x400643FC  ;GPIO Port N Data Register address
	
GPIO_PORTM_DIR_R        EQU		0x40063400  ;GPIO Port M Direction Register Address 
GPIO_PORTM_DEN_R        EQU		0x4006351C  ;GPIO Port M Direction Register Address 
GPIO_PORTM_DATA_R       EQU		0x400633FC  ;GPIO Port M Data Register Address      
GPIO_PORTF_DIR_R        EQU		0x4005D400  ;GPIO Port M Direction Register Address 
GPIO_PORTF_DEN_R        EQU		0x4005D51C  ;GPIO Port M Direction Register Address 
GPIO_PORTF_DATA_R       EQU		0x4005D3FC  ;GPIO Port M Data Register Address   
	
;Define constants

COMBINATION			EQU		2_1101	; this is the sequential combination expected - does NOT include the clock
COMBINATION_LENGTH	EQU		4		; number of digits in combination (decimal number)
RESET_STATE			EQU		2_0000
CLOCK_BIT			EQU		2_100	; clock is on PM2                            



        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortM_Init
;Enable Port M and set bits PM0-PM2 for digital input
PortM_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x800				;Modifies the contents of R0 to set bit 11 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 	
		;STEP 3: Set Port Direction (set bit 0 to 0 for input)
		LDR R1, =GPIO_PORTM_DIR_R		;Load the memory address of the GPIODIR Port M Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port M Register into R0
		BIC R0, R0, #0x07				;Modify the contents of R0 to clear bits PM0-PM2 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 0 to 1 for digital enable)
		LDR R1, =GPIO_PORTM_DEN_R		;Load the memory address of the GPIODEN Port M Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port M Register into R0
		ORR R0, R0, #0x07				;Modify the contents of R0 to set bits PM0-PM2 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 

;Function PortN_Init
;Enable PortN and set bits PN0 and PN1 for digital output
PortN_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x1000				;Modifies the contents of R0 to set bit 12 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 4 to 1 for output)
		LDR R1, =GPIO_PORTN_DIR_R		;Load the memory address of the GPIODIR Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port N Register into R0
		ORR R0, R0, #0x3				;Modify the contents of R0 to set bits PN0-PN1 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 4 to 1 for digital enable)
		LDR R1, =GPIO_PORTN_DEN_R		;Load the memory address of the GPIODEN Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port N Register into R0
		ORR R0, R0, #0x3				;Modify the contents of R0 to set bits PN0-PN1 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 
		
PortF_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x20				;Modifies the contents of R0 to set bit 12 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 4 to 1 for output)
		LDR R1, =GPIO_PORTF_DIR_R		;Load the memory address of the GPIODIR Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port N Register into R0
		ORR R0, R0, #0x10				;Modify the contents of R0 to set bits PN0-PN1 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 4 to 1 for digital enable)
		LDR R1, =GPIO_PORTF_DEN_R		;Load the memory address of the GPIODEN Port N Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port N Register into R0
		ORR R0, R0, #0x10				;Modify the contents of R0 to set bits PN0-PN1 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 
       
;define the states
Locked_State
		LDR R1, =GPIO_PORTF_DATA_R		
		LDR R0, [R1]					
		BIC R0, R0, #0x10				;Clear bit 1 - PN1 controls D1, OFF for Locked
		STR R0, [R1]					

		MOV R5, #COMBINATION			;Temporarily store combination in R5
		LSR R5, R8						;Eliminate digits that have not yet been entered
		CMP R5, R9						;Check that combination is still on track
		BEQ Keep_Going					;If we are, go back to main program
		BNE Reset
		BX LR
Reset
		LDR R1, =GPIO_PORTN_DATA_R
		MOV R6, #0
		STR R6, [R1]
		MOV R9, #RESET_STATE			;Otherwise, reset everything before returning
		MOV R8, #COMBINATION_LENGTH
		BX LR
		
Keep_Going
		LDR R1, =GPIO_PORTN_DATA_R		
		ADD R6, #0x1
		STR R6, [R1]
		BX LR
		
Unlocked_State
		LDR R1, =GPIO_PORTF_DATA_R		
		LDR R0, [R1]					
		ORR R0, R0, #0x10
		STR R0, [R1]
		LDR R1, =GPIO_PORTN_DATA_R
		MOV R6, #0
		STR R6, [R1]
		BX LR

WaitForClockHigh 
		LDR R1, =GPIO_PORTM_DATA_R		;Load the memory address of the GPIODATA Port M Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODATA Port M Register into R0
		ANDS R2, R0, #CLOCK_BIT			;Stores result in R2 to avoid changing R0. Result is 0 iff the clock bit is 0.
										;The 'S' modifier updates the flags. If result is 0, Z == 1 and condition EQ is true.
		BEQ WaitForClockHigh			;If it's 0, keep waiting

		LDR R7, =1200000
Debounce
		SUB R7, R7, #1
		CMP R7, #0
		BNE Debounce
		
		BX LR

WaitForClockLow 
		LDR R1, =GPIO_PORTM_DATA_R		;Load the memory address of the GPIODATA Port M Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODATA Port M Register into R0
		ANDS R2, R0, #CLOCK_BIT			;Stores result in R2 to avoid changing R0. Result is 0 iff the clock bit is 0.
										;The 'S' modifier updates the flags. If result is 0, Z == 1 and condition EQ is true.
		BNE WaitForClockLow				;If it's not 0, keep waiting
		BX LR


Start 
		BL PortF_Init
	    BL PortM_Init       ; call and execute PortF_Init
		BL PortN_Init		; call and execute PortL_Init
		
Initialize_State
		LDR R1, =GPIO_PORTN_DATA_R
		MOV R6, #0
		STR R6, [R1]
		LDR R1, =GPIO_PORTF_DATA_R
		STR R6, [R1]
		BL Locked_State					; initial state is unlocked		
		MOV R9, #RESET_STATE			;R9 will hold the part of the combination entered so far
		MOV R8, #COMBINATION_LENGTH		;R8 will hold the number of digits yet to be entered

FSM
		BL WaitForClockLow				;First make sure the clock is low
		BL WaitForClockHigh				;Wait for rising edge: low -> high. Now the input is valid and is already stored in R0

		CMP R8, #0
		BEQ Initialize_State

		AND R0, R0, #0x01		;Get rid of contents of R0 except LSB
		LSL R9, #1				;Shift contents of R9 to make room for new bit
		ORR R9, R0				;Add LSB of R0 to R9
		SUB R8, #1				;Decrement the number of digits yet to be entered
		
		CMP R9, #COMBINATION	;Compare with the valid combination
		BLEQ Unlocked_State
		BLNE Locked_State

		B FSM


		ALIGN               ; directive for assembly			
        END                 ; End of function 