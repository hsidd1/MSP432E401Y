SYSCTL_RCGCGPIO_R             EQU     0x400FE608         ;Step 1: GPIO Run Mode Clock Gating Control Register Address
GPIO_PORTF_DIR_R              EQU     0x4005D400         ;Step 3: GPIO Port F DIR Register Address
GPIO_PORTF_DEN_R              EQU     0x4005D51C         ;Step 4: GPIO Port F DEN Register Address
GPIO_PORTF_DATA_R             EQU     0x4005D3FC         ;Step 5: GPIO Port F DATA Register Address
GPIO_PORTL_DIR_R	 		  EQU	  0x40062400
GPIO_PORTL_DEN_R			  EQU	  0x4006251C
GPIO_PORTL_DATA_R			  EQU	  0x400623FC
GPIO_PORTN_DIR_R	 		  EQU	  0x40064400
GPIO_PORTN_DEN_R			  EQU	  0x4006451C
GPIO_PORTN_DATA_R			  EQU	  0x400643FC
GPIO_PORTM_DIR_R	 		  EQU	  0x40063400
GPIO_PORTM_DEN_R			  EQU	  0x4006351C
GPIO_PORTM_DATA_R			  EQU	  0x400633FC
                              



        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortF_Init
;Enable PortF and set bit PF4 for digital output
PortN_Init 
		;STEP 1 Activate clock (Set bit 5 in RCGCGPIO; In C pseudcode: SYSCTL_RCGCGPIO_R |= #0x20)
		 LDR R1, =SYSCTL_RCGCGPIO_R		;Stores the address of the RCGCGPIO register in R1
		 LDR R0, [R1]					;Dereferences R1 to put the contents of the RCGCGPIO register in R0
		 ORR R0,R0, #0x20				;Modifies the contents of R0 to set bit 5 without changing other bits
		 STR R0, [R1]					;Stores the new value into the RCGCGPIO register
		
		;STEP 2: Wait for Peripheral Ready
		 NOP
		 NOP
		 
		
		LDR R1, =GPIO_PORTF_DIR_R	;STEP 3: Set Port Direction (set bit 4 to 1 for output)
		LDR R0, [R1]			;Load the memory address of the GPIODIR Port F Register into R1 (pointer)
		ORR R0,R0, #0x10			;Load the contents from the memory address of GPIODIR Port F Register into R0
		STR R0, [R1]			;Modify the contents of R0 to set bit 4 without changing other bits
					;Store what is in R0 into address pointed by R1 
		 
		LDR R1, =GPIO_PORTF_DEN_R  ;STEP 4: Enable Digital Functioning (set bit 4 to 1 for digital enable)
		LDR R0, [R1]			;Load the memory address of the GPIODEN Port F Register into R1 (pointer)
		ORR R0,R0, #0x10			;Load the contents from the memory address of GPIODEN Port F Register into R0
		STR R0, [R1]			;Modify the contents of R0 to set bit 4 without changing other bits
					;Store what is in R0 into address pointed by R1 

        BX LR               ; return from function

PortM_Init
	
		LDR R1, =SYSCTL_RCGCGPIO_R
		LDR R0, [R1]
		ORR R0,R0, #0x800
		STR R0, [R1]
		
		NOP
		NOP
		
		LDR R1, =GPIO_PORTM_DIR_R
		LDR R0, [R1]
		AND R0,R0, #0xFE
		STR R0, [R1]
		
		LDR R1, =GPIO_PORTM_DEN_R
		LDR R0, [R1]
		ORR R0,R0, #0x1
		STR R0, [R1]
		
		BX LR

 
WaitForButtonPress 

		LDR R1, =GPIO_PORTM_DATA_R				;Load the memory address of the GPIODATA Port M Register into R1 (pointer)
		LDR R0, [R1]							;Load the contents from the memory address of GPIODATA Port M Register into R0
		AND R0, R0, #0x01						;Clear all bits in R0 except bit 0, which is the bit that we need to check
		CMP R0, #1								;Compare the result with 0
		BNE WaitForButtonPress	 
		BL LightON
		BL WaitForButtonRelease
   
LightON  
		LDR R1, =GPIO_PORTF_DATA_R
		LDR R0, [R1]
		ORR R0, R0, #0x10
		STR R0, [R1] 
		
		BX LR
		
LightOFF
		LDR R1, =GPIO_PORTF_DATA_R
		LDR R0, [R1]
		AND R0, R0, #0xEF
		STR R0, [R1] 
		
		BX LR
   
WaitForButtonRelease   

		LDR R1, =GPIO_PORTM_DATA_R
		LDR R0, [R1]
		AND R0, R0, #0x01
		CMP R0, #0
		BNE WaitForButtonRelease
		BL LightOFF
		BL WaitForButtonPress

 
BlinkON

		LDR R1, =GPIO_PORTF_DATA_R
		LDR R0, [R1]
		ORR R0, R0, #0x1
		STR R0, [R1]
		BL Delay
		BL BlinkOFF
		
		
BlinkOFF

		LDR R1, =GPIO_PORTF_DATA_R
		LDR R0, [R1]
		AND R0, R0, #0xFFFFFFFE
		STR R0, [R1]
		BL Delay
		BL BlinkON
		

Delay
		
		ADD R0, R0, #1
		CMP R0, #1000
		BLE Delay
		BX LR


Start 
	    BL PortN_Init       ; call and execute PortF_Init
		BL PortM_Init
		
		BL WaitForButtonPress
		;BL BlinkON
		
		 
		ALIGN               ; directive for assembly			
        END                 ; End of function 