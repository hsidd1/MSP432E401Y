;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;BLINK PROGRAM - HAMZA SIDDIQUI, MOHAMED BISHR HAMIDEH



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS

;The EQU directive gives a symbolic name to a numeric constant, a register-relative value or a program-relative value


SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
GPIO_PORTF_DIR_R             EQU 0x4005D400  ;GPIO Port F Direction Register address 
GPIO_PORTF_DEN_R             EQU 0x4005D51C  ;GPIO Port F Digital Enable Register address
GPIO_PORTF_DATA_R            EQU 0x4005D3FC  ;GPIO Port F Data Register address

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Do not alter this section

        AREA    |.text|, CODE, READONLY, ALIGN=2 ;code in flash ROM
        THUMB                                    ;specifies using Thumb instructions
        EXPORT Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Define Functions in your code here 
;The function Port F_Init to configures the GPIO Port F Pin 4 for Digital Output 

PortF_Init
    ; STEP 1
    LDR R1, =SYSCTL_RCGCGPIO_R          ;Loads the memory address of RCGCGPIO into register 1(R1); R1 = 0x400FE608
    LDR R0, [R1]                        ;Put the contents of the memory address of RCGCGPIO into register 0 (R0), R0 = 0x00000000
    ORR R0,R0, #0x20                    ;Performs a bitwise OR operation with the contents of R0 and 0x20 and stores it back into R0, R0 = 0x20 
    STR R0, [R1]                        ;Stores R0 contents into contents of the address located in R1,RCGCGPIO now has Ox20 stored in it 
    NOP                                 ;Waiting for GPIO Port F to be enabled
	NOP                                 ;Waiting for GPIO Port F to be enabled
  
   ; STEP 5
    LDR R1, =GPIO_PORTF_DIR_R           ;Load the memory address of the GPIO Port F DIR Register into register 1 (R1), R1 = 0x4005D400
    LDR R0, [R1]                        ;Put the contents of the memory address of GPIO Port F DIR Register in R0, R0 = 0x00000000
    ORR R0,R0, #0x10                  	;Perform a bitwise OR operation with the contents of R0 with 0x10 and put the contents into R0 , R0 = 0x10
    STR R0, [R1]                        ;Stores R0 contents into contents of the address located in R1; GPIO Port F Direction Register now has 0x10 stored in it 
    
	; STEP 7
    LDR R1, =GPIO_PORTF_DEN_R           ;Load the memory addess of the GPIO Port F DEN Register into register 1 (R1), R1 = 0x4005D51C
    LDR R0, [R1]                        ;Put the contents of the memory address of GPIO Port F DEN Register in register 0 (R0,), R0 = 0x00000000
    ORR R0, R0, #0x10                   ;Perform a bitwise OR operation with the contents of R0 with 0x10 and put the contents into R0, R0 = 0x10 
    STR R0, [R1]	                    ;Stores R0 contents into contents of the address located in R1; GPIO Port F DEN Register now has 0x10 stored in it 
    BX  LR                              ;return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Start
	BL  PortF_Init                      ;The BL instruction is like a function call 
    ;STEP 8                              		
    LDR R1, =GPIO_PORTF_DATA_R          ;Load the memory addess of the GPIO Port F DATA Register into register 1 (R1), R1 = 0x4005D3FC
	LDR R0,[R1]                         ;Put the contents of the memory address of GPIO Port F DATA Register 0 (R0), R0 = 0x00000000
    ORR R0,R0, #0x10                    ;Perform a bitwise OR operation with the contents of R0 with 0x10 and put the contents into R0, R0 = 0x10
    STR R0, [R1] ;Stores R0 contents into contents of the address located in R1; GPIO Port F Data Register now has 0x10 stored in it 	
branchA	LDR R1, =GPIO_PORTF_DATA_R      ; Start of branchA
	LDR R0,[R1]                         
    EOR R0,R0, #0x10                    ;Causes the LED to toggle on and off
    STR R0, [R1]
	LDR R2, =2000000
delay SUBS R2, R2, #1
	BNE delay
	B	branchA							;Branch to 'branchA'
	ALIGN                               ;Do not touch this 
    END   