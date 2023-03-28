// 2DX4_Knowledge_Thread_3_Session_1
// This program illustrates the use of SysTick in the C language.
// Note the library headers asscoaited are PLL.h and SysTick.h,
// which define functions and variables used in PLL.c and SysTick.c.
// This program uses code directly from your course textbook.

//  Written by Ama Simons
//  January 18, 2020
//  Last Update: January 18, 2020


#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"



void PortM_Init(void){
	//Use PortM pins for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};	// allow time for clock to stabilize
	GPIO_PORTM_DIR_R |= 0xFF;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTM_AFSEL_R &= ~0xFF;     								// disable alt funct on PN0
  GPIO_PORTM_DEN_R |= 0xFF;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTM_PCTL_R = (GPIO_PORTM_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTM_AMSEL_R &= ~0xFF;     								// disable analog functionality on PN0		
	return;
}

void PortH_Init(void){
    //Use PortM pins for output
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;                // activate clock for Port N
    while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R7) == 0){};   // allow time for clock to stabilize
    GPIO_PORTH_DIR_R |= 0xFF;                                       // make PN0 out (PN0 built-in LED1)
  GPIO_PORTH_AFSEL_R &= ~0xFF;                                  // disable alt funct on PN0
  GPIO_PORTH_DEN_R |= 0xFF;                                     // enable digital I/O on PN0
                                                                                                    // configure PN1 as GPIO
  //GPIO_PORTM_PCTL_R = (GPIO_PORTM_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTH_AMSEL_R &= ~0xFF;                                  // disable analog functionality on PN0      
    return;
}

int wait = 190000;

void spin(int direction){
	if(direction == -1){
		for(int i=0; i<512; i++){
			GPIO_PORTH_DATA_R = 0b00001001;
			SysTick_Wait(wait);
			GPIO_PORTH_DATA_R = 0b00000011;
			SysTick_Wait(wait);
			GPIO_PORTH_DATA_R = 0b00000110;
			SysTick_Wait(wait);
			GPIO_PORTH_DATA_R = 0b00001100;
			SysTick_Wait(wait);
		}
	}
	if(direction == 1){
		for(int i=0; i<512; i++){
		GPIO_PORTH_DATA_R = 0b00001100;
		SysTick_Wait(wait);
		GPIO_PORTH_DATA_R = 0b00000110;
		SysTick_Wait(wait);
		GPIO_PORTH_DATA_R = 0b00000011;
		SysTick_Wait(wait);
		GPIO_PORTH_DATA_R = 0b00001001;
		SysTick_Wait(wait);
		}
	}
}

int main(void){
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortH_Init();	
	spin(1);
	SysTick_Wait10ms(100);
	spin(-1);
	return 0;
}






