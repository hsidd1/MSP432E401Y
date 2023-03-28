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

void LED_PWM_BLINKR(int msperiod, int duty){

	int tenusperiod = msperiod*100;												//Changes period from ms units to 10ms units
	int onduty = (duty*tenusperiod)/100;									//Sets the onduty period, where duty was given as a number from 0-100
	int offduty = tenusperiod - (duty*tenusperiod)/100;		//Sets offduty period
	GPIO_PORTM_DATA_R ^= 0b00000100;										//Toggles LED
	SysTick_Wait10us(onduty);														//Waits for onduty
	GPIO_PORTM_DATA_R ^= 0b00000100;										//Toggles LED
	SysTick_Wait10us(offduty);													//Waits for offduty
}

void LED_PWM_BLINKG(int msperiod, int duty){

	int tenusperiod = msperiod*100;												//Changes period from ms units to 10ms units
	int onduty = (duty*tenusperiod)/100;									//Sets the onduty period, where duty was given as a number from 0-100
	int offduty = tenusperiod - (duty*tenusperiod)/100;		//Sets offduty period
	GPIO_PORTM_DATA_R ^= 0b00000010;										//Toggles LED
	SysTick_Wait10us(onduty);														//Waits for onduty
	GPIO_PORTM_DATA_R ^= 0b00000010;										//Toggles LED
	SysTick_Wait10us(offduty);													//Waits for offduty
}

void LED_PWM_BLINKB(int msperiod, int duty){

	int tenusperiod = msperiod*100;												//Changes period from ms units to 10ms units
	int onduty = (duty*tenusperiod)/100;									//Sets the onduty period, where duty was given as a number from 0-100
	int offduty = tenusperiod - (duty*tenusperiod)/100;		//Sets offduty period
	GPIO_PORTM_DATA_R ^= 0b00000001;										//Toggles LED
	SysTick_Wait10us(onduty);														//Waits for onduty
	GPIO_PORTM_DATA_R ^= 0b00000001;										//Toggles LED
	SysTick_Wait10us(offduty);													//Waits for offduty
}



int main(void){

	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortM_Init();																		// Initialize Port N

	uint8_t dutyr = 100;
	uint8_t dutyg = 100;
	uint8_t dutyb = 100;
	uint8_t period = 10;
	while(1){
		LED_PWM_BLINKR(period,dutyb);
		LED_PWM_BLINKG(period,dutyg);
		LED_PWM_BLINKB(period,dutyr);
	}
}