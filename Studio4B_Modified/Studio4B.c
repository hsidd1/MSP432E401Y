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

uint32_t WAIT = 400000;

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

void spin(){
	for(int i=0; i<512; i++){
		GPIO_PORTM_DATA_R = 0b00001100;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00000110;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00000011;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00001001;
		SysTick_Wait(WAIT);
	}
}
void spinopp(){
	for(int i=0; i<512; i++){
		GPIO_PORTM_DATA_R = 0b00001001;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00000011;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00000110;
		SysTick_Wait(WAIT);
		GPIO_PORTM_DATA_R = 0b00001100;
		SysTick_Wait(WAIT);
	}
}

int main(void){
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortM_Init();	
	for (int i = 0; i < 5; i++){
	spin();
	spinopp();
	}
	return 0;
}






