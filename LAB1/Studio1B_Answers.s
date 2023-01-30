;Template - Defining an Assembly File 
;
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
;
; Modified by: Yaser M. Haddara
; January 21, 2023
; Program for studio 1A and 1B
; 1A: switch on D3
; 1B: use a pushbutton to control a user LED
;
; McMaster University, Comp Eng 2DX3, Winter 2023
; Further resources: Valvano textbook, Chs. 3 & 4
;

;ADDRESS SETUP
;Define your I/O Port Addresses Here

SYSCTL_RCGCGPIO_R             EQU     0x400FE608         ;Step 1: GPIO Run Mode Clock Gating Control Register Address
GPIO_PORTF_DIR_R              EQU     0x4005D400         ;Step 3: GPIO Port F DIR Register Address
GPIO_PORTF_DEN_R              EQU     0x4005D51C         ;Step 4: GPIO Port F DEN Register Address
GPIO_PORTF_DATA_R             EQU     0x4005D3FC         ;Step 5: GPIO Port F DATA Register Address

GPIO_PORTL_DIR_R              EQU     0x40062400         ;Step 3: GPIO Port L DIR Register Address
GPIO_PORTL_DEN_R              EQU     0x4006251C         ;Step 4: GPIO Port L DEN Register Address
GPIO_PORTL_DATA_R             EQU     0x400623FC         ;Step 5: GPIO Port L DATA Register Address
                              



        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortF_Init
;Enable PortF and set bit PF4 for digital output
PortF_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x20				;Modifies the contents of R0 to set bit 5 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 4 to 1 for output)
		LDR R1, =GPIO_PORTF_DIR_R		;Load the memory address of the GPIODIR Port F Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port F Register into R0
		ORR R0, R0, #0x10				;Modify the contents of R0 to set bit 4 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 4 to 1 for digital enable)
		LDR R1, =GPIO_PORTF_DEN_R		;Load the memory address of the GPIODEN Port F Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port F Register into R0
		ORR R0, R0, #0x10				;Modify the contents of R0 to set bit 4 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 
       
;Function PortL_Init
;Enable PortL and set bit PL0 for digital input
PortL_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x400				;Modifies the contents of R0 to set bit 10 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		;STEP 3: Set Port Direction (set bit 0 to 0 for input)
		LDR R1, =GPIO_PORTL_DIR_R		;Load the memory address of the GPIODIR Port L Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODIR Port L Register into R0
		AND R0, R0, #0xFE				;Modify the contents of R0 to clear bit 0 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 
		 
		;STEP 4: Enable Digital Functioning (set bit 0 to 1 for digital enable)
		LDR R1, =GPIO_PORTL_DEN_R		;Load the memory address of the GPIODEN Port L Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODEN Port L Register into R0
		ORR R0, R0, #0x01				;Modify the contents of R0 to clear bit 0 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function 



Start 
	    BL PortF_Init       ; call and execute PortF_Init
		BL PortL_Init		; call and execute PortL_Init
		
		;STEP 5: Outputting a Logic High to Port F Pin 4 (to switch on D3)   
LightON
		LDR R1, =GPIO_PORTF_DATA_R		;Load the memory address of the GPIODATA Port F Register into R1 (pointer)
		LDR R0, [R1]					;Load the contents from the memory address of GPIODATA Port F Register into R0
		ORR R0, R0, #0x10				;Modify the contents of R0 to set bit 4 without changing other bits
		STR R0, [R1]					;Store what is in R0 into address pointed by R1 

		;We now need to read the input (pin PL0), check if it is 0,
		;switch off the LED if it is 0, otherwise keep the LED ON
		
WaitForButtonPress 
		LDR R1, =GPIO_PORTL_DATA_R	;Load the memory address of the GPIODATA Port L Register into R1 (pointer)
		LDR R0, [R1]							;Load the contents from the memory address of GPIODATA Port L Register into R0
		AND R0, R0, #0x01						;Clear all bits in R0 except bit 0, which is the bit that we need to check
		CMP R0, #0								;Compare the result with 0
		BNE WaitForButtonPress					;If it's not 0, keep waiting
		
		LDR R1, =GPIO_PORTF_DATA_R		;We only get here when the button has been pressed, so switch OFF the LED
		LDR R0, [R1]
		AND R0, R0, #0xEF				;Clear bit 4 on Port F to switch off D3
		STR R0, [R1]
		
WaitForButtonRelease 
		LDR R1, =GPIO_PORTL_DATA_R		;Go through a similar sequence to what we did to check if button was pushed
		LDR R0, [R1]
		AND R0, R0, #0x01
		CMP R0, #0x1								;The only difference is we're comparing with 1 (button released) instead of 0
		BNE WaitForButtonRelease
		B LightON									;We only get here when button is released, so go back to beginning						;


		ALIGN               ; directive for assembly			
        END                 ; End of function 