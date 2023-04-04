//  Name: Mohammad Bishr Hamideh, 400408676, hamidehm
// 	Name: Hamza Siddiqui, 400407170, siddih38
// 	Lab Section: L01
//  February 26, 2023


#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"


void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;						// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x0F;        										// make PN0 out (PN0 built-in LED1)
  GPIO_PORTN_AFSEL_R &= ~0x0F;     										// disable alt funct on PN0
  GPIO_PORTN_DEN_R |= 0x0F;        										// enable digital I/O on PN0
																											// configure PN1 as GPIO
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x0F;     										// disable analog functionality on PN0
	
	GPIO_PORTN_DATA_R ^= 0b00000001;
	SysTick_Wait10ms(10);																//.1s delay
	GPIO_PORTN_DATA_R ^= 0b00000001;	
	return; 
}


void LED_PWM_BLINK(int msperiod, int duty){

	int tenusperiod = msperiod*100;												//Changes period from ms units to 10ms units
	int onduty = (duty*tenusperiod)/100;									//Sets the onduty period, where duty was given as a number from 0-100
	int offduty = tenusperiod - (duty*tenusperiod)/100;		//Sets offduty period
	GPIO_PORTN_DATA_R ^= 0b00001111;										//Toggles LED
	SysTick_Wait10us(onduty);														//Waits for onduty
	GPIO_PORTN_DATA_R ^= 0b00001111;										//Toggles LED
	SysTick_Wait10us(offduty);													//Waits for offduty
}

int main(void){
	
	PLL_Init();																			// Default Set System Clock to 120MHz
	SysTick_Init();																	// Initialize SysTick configuration
	PortN_Init();																		// Initialize Port N 
	
	uint8_t duty = 50;															//50% duty cycle
	uint32_t period = 10;														//Student number ends with 6, therefore period is 500ms
	while(1){
		for(int i = 0; i<10;i++){
			for(int j = 0; j<10;j++){
				LED_PWM_BLINK(period,i*10);
			}
		}
		for(int i = 0; i<10;i++){
			for(int j = 0; j<10;j++){
				LED_PWM_BLINK(period,(100-(i*10)));
			}
		}									//Loops blinker
	}
}





