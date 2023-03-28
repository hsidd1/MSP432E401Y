/*****************************************************************************************************************************************
Description of Code: Milestone 1 Lab 7. Periodic interrupt of 500ms with ISR that flashes onboard LED with 100ms pulse.

Reference: Studio W7-1 Project Code
*******************************************************************************************************************************************/
#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

void FlashLED2(int count) {                           //Flash D2
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000101; 								// Flash onvoard LED D2 as well as PN2 to inspect on AD2
			SysTick_Wait10ms(25);														//.1s delay (100ms)
			GPIO_PORTN_DATA_R ^= 0b00000101;			
		}
}

void FlashLED1(int count) {														//Flash D1
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000110; 								// Flash onvoard LED D1 as well as PN2 to inspect on AD2
			SysTick_Wait10ms(25);														//.1s delay (100ms)
			GPIO_PORTN_DATA_R ^= 0b00000110;			
		}
}

// Initialize onboard LEDs
void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x07;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTN_AFSEL_R &= ~0x07;     								// disable alt funct on PN0
  GPIO_PORTN_DEN_R |= 0x07;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO

  GPIO_PORTN_AMSEL_R &= ~0x07;     								// disable analog functionality on PN0		
	FlashLED1(1);
	return;
}

// Enable interrupts
void EnableInt(void)
{    __asm("    cpsie   i\n");
}

// Disable interrupts
void DisableInt(void)
{    __asm("    cpsid   i\n");
}

// Low power wait
void WaitForInt(void)
{    __asm("    wfi\n");
}

void Timer3_Init(void){
	// Assumes 120MHz bus, bus period = 1/(120*10^6)
	// If bus clock = timer clock, then max interrupt period = 1/(120*10^6) * 2^32 = 35.8s
	// We *choose* for this eample a timer period of .1s
	// Since we multiply the period by 120 to match the units of 1 us, the period will be in 1us units
	// 0.5s = 500,000
	uint32_t period=1000000; 												// 32-bit value in 1us increments
	
	SYSCTL_RCGCTIMER_R |= 0x0008;						//Activate timer 
	SysTick_Wait10ms(1);
	TIMER3_CTL_R = 0x00000000;							//Disable timer3 during setup 
	TIMER3_CFG_R = 0x00000000;							//Configure for 32-bit timer mode   
	TIMER3_TAMR_R = 0x00000002;							//Configure for periodic mode   
	TIMER3_TAILR_R = (period*120)-1; 				//Reload value (we multiply the period by 120 to match the units of 1 us)  
	TIMER3_TAPR_R = 0;											//8.3ns timer3 max   
	TIMER3_ICR_R = 0x00000001;		 					// clear timer3 timeout flag   
	TIMER3_IMR_R |= 0x00000001;							// arm timeout interrupt   
	NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x40000000;		//Priority 2 
	NVIC_EN1_R = 1<<(35-32);								//Enable IRQ 35 in NVIC   
	TIMER3_CTL_R |= 0x00000001;							//Enable timer3   
	EnableInt();
} 

void TIMER3A_IRQHandler(void){
	TIMER3_ICR_R = 0x00000001;							// acknowledge timer3 timeout   
	FlashLED2(1);														// execute user task -- we simply flash LED
}

int main(void){
  PLL_Init();               // set system clock to 120 MHz
	SysTick_Init();
	PortN_Init();
	Timer3_Init();
	
	while(1){
		WaitForInt();
	}
}
