;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; Name: HAMZA SIDDIQUI, MOHAMED BISHR HAMIDEH; 
; Student Number: 400407170, 400408676
; Lab Section: L01
; Description of Code: This code allows User LED D2 To Turn on Using Port N
; Last Modified: January 30, 2023


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;ADDRESS DEFINTIONS

;The EQU directive gives a symbolic name to a numeric constant, a register-relative value or a program-relative value


SYSCTL_RCGCGPIO_R            EQU 0x400FE608  ;General-Purpose Input/Output Run Mode Clock Gating Control Register (RCGCGPIO Register)
GPIO_PORTN_DIR_R             EQU 0x40064400  ;GPIO Port N Direction Register address 
GPIO_PORTN_DEN_R             EQU 0x4006451C  ;GPIO Port N Digital Enable Register address
GPIO_PORTN_DATA_R            EQU 0x400643FC  ;GPIO Port N Data Register address

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Do not alter this section

        AREA    |.text|, CODE, READONLY, ALIGN=2 ;code in flash ROM
        THUMB                                    ;specifies using Thumb instructions
        EXPORT Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Define Functions in your code here 
;The function Port F_Init to configures the GPIO Port F Pin 4 for Digital Output 

PortN_Init
    ; STEP 1
    LDR R1, =SYSCTL_RCGCGPIO_R          ;Loads the memory address of RCGCGPIO into register 1(R1); R1 = 0x400FE608
    LDR R0, [R1]                        ;Put the contents of the memory address of RCGCGPIO into register 0 (R0), R0 = 0x00000000
    ORR R0,R0, #0x1000                  ; R12 -- > Performs a bitwise OR operation with the contents of R0 
    STR R0, [R1]                        ;Stores R0 contents into contents of the address located in R1,RCGCGPIO now has Ox20 stored in it 
    NOP                                 ;Waiting for GPIO Port N to be enabled
	NOP                                 ;Waiting for GPIO Port N to be enabled
  
    LDR R1, =GPIO_PORTN_DIR_R           ;Load the memory address of the GPIO Port N DIR Register into register 1 (R1)
    LDR R0, [R1]                        ;Put the contents of the memory address of GPIO Port N DIR Register in R0, R0 = 0x00000000
    ORR R0,R0, #0x01                  	;Perform a bitwise OR operation with the contents of R0 with 0x01 and put the contents into R0 , R0 = 0x01
    STR R0, [R1]                        ;Stores R0 contents into contents of the address located in R1; GPIO Port N Direction Register now has 0x01 stored in it 
    
    LDR R1, =GPIO_PORTN_DEN_R           ;Load the memory addess of the GPIO Port N DEN Register into register 1 (R1)
    LDR R0, [R1]                        ;Put the contents of the memory address of GPIO Port N DEN Register in register 0 (R0,), R0 = 0x00000000
    ORR R0, R0, #0x01                    ;Perform a bitwise OR operation with the contents of R0 with 0x01 and put the contents into R0
    STR R0, [R1]	                    ;Stores R0 contents into contents of the address located in R1; GPIO Port N DEN Register now has 0x01 stored in it 
    BX  LR                              ;return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

Start
	BL  PortN_Init                      ;The BL instruction is like a function call 
    LDR R1, =GPIO_PORTN_DATA_R          ;Load the memory addess of the GPIO Port N DATA Register into register 1 (R1), R1 = 0x400643FC
	LDR R0,[R1]                         ;Put the contents of the memory address of GPIO Port N DATA Register 0 (R0), R0 = 0x00000000
    ORR R0,R0, #0x01                    ;Perform a bitwise OR operation with the contents of R0 with 0x01 and put the contents into R0, R0 = 0x01
    STR R0, [R1] 						;Stores R0 contents into contents of the address located in R1; GPIO Port N Data Register now has 0x10 stored in it 	
branchA	LDR R1, =GPIO_PORTN_DATA_R      ; Start of branchA
	LDR R0,[R1]                         
    EOR R0,R0, #0x01                    ;Causes the LED to toggle on and off
    STR R0, [R1]
	LDR R2, =2000000
delay SUBS R2, R2, #1
	BNE delay
	B	branchA							;Branch to 'branchA'
	ALIGN                               ;Do not touch this 
    END   