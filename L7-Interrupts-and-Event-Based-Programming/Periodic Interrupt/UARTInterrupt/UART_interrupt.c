#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

void UART_Init(void) {
    SYSCTL_RCGCUART_R |= 0x0001; // activate UART0   
    SYSCTL_RCGCGPIO_R |= 0x0001; // activate port A   
    //UART0_CTL_R &= ~0x0001; // disable UART   
 
    while((SYSCTL_PRUART_R&SYSCTL_PRUART_R0) == 0){};
        
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
 
    UART0_CC_R = (UART0_CC_R&~UART_CC_CS_M)+UART_CC_CS_PIOSC;
                                                                                // UART gets its clock from the alternate clock source 
                                                                                // as defined by SYSCTL_ALTCLKCFG_R
  SYSCTL_ALTCLKCFG_R = (SYSCTL_ALTCLKCFG_R&~SYSCTL_ALTCLKCFG_ALTCLK_M)+SYSCTL_ALTCLKCFG_ALTCLK_PIOSC;
                                        // the alternate clock source is the PIOSC (default)
    UART0_CTL_R &= ~UART_CTL_HSE;         // high-speed disable; divide clock by 16 rather than 8 (default)
 
    UART0_IBRD_R = 8;                     // IBRD = int(16,000,000 / (16 * 115,200)) = int(8.681)
  UART0_FBRD_R = 44;                    // FBRD = round(0.6806 * 64) = 44
    UART0_LCRH_R = 0x0070;                              // 8-bit word length, enable FIFO   
 
    UART0_CTL_R = 0x0301;           // enable RXE, TXE and UART   
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011; // UART   
    GPIO_PORTA_AMSEL_R &= ~0x03;    // disable analog function on PA1-0   
    GPIO_PORTA_AFSEL_R |= 0x03;     // enable alt funct on PA1-0   
    GPIO_PORTA_DEN_R |= 0x03;           // enable digital I/O on PA1-0 
}




//Flash D1
void FlashLED1(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000010; 								//hello world!
			SysTick_Wait10ms(10);														//.1s delay
			GPIO_PORTN_DATA_R ^= 0b00000010;			
		}
}

//Flash D2
void FlashLED2(int count) {
		while(count--) {
			GPIO_PORTN_DATA_R ^= 0b00000001; 								//hello world!
			SysTick_Wait10ms(10);														//.1s delay
			GPIO_PORTN_DATA_R ^= 0b00000001;			
		}
}

void PortN_Init(void){
	//Use PortN onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				// Activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};	// Allow time for clock to stabilize
	GPIO_PORTN_DIR_R |= 0x03;        								// Make PN0 and PN1 output (Built-in LEDs: D1 (PN1) and D2 (PN0))
  GPIO_PORTN_AFSEL_R &= ~0x03;     								// Disable alt funct on PN0 and PN1
  GPIO_PORTN_DEN_R |= 0x03;        								// Enable digital I/O on PN0 and PN1
																									
  //GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFFFF00)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x03;     								// Disable analog functionality on PN0 and PN1
	FlashLED1(1);
	return;
}



void UART_OutChar(char data){
        while((UART0_FR_R&0x0020) != 0);    // wait until TXFF is 0   
        UART0_DR_R = data;
    } 
    
void UART_OutString(char* data){
        int i=0;
        while(data[i]!=0){
      UART_OutChar(data[i]);  // add one line of code to complete this function
      i = i+1;
        }
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

// Global variable visible in Watch window of debugger
// increments at least once per button press
volatile unsigned long FallingEdges = 0;

// Give clock to Port J and initalize as input GPIO
void PortJ_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;					// Activate clock for Port J
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0){};	// Allow time for clock to stabilize
  GPIO_PORTJ_DIR_R &= ~0x03;    										// Make PJ1 input 
  GPIO_PORTJ_DEN_R |= 0x03;     										// Enable digital I/O on PJ1
	
	GPIO_PORTJ_PCTL_R &= ~0x000000FF;	 								//? Configure PJ1 as GPIO 
	GPIO_PORTJ_AMSEL_R &= ~0x03;											//??Disable analog functionality on PJ1		
	GPIO_PORTJ_PUR_R |= 0x03;													//	Enable weak pull up resistor
}
// Interrupt initialization for GPIO Port J IRQ# 51
void PortJ_Interrupt_Init(void){
	
		FallingEdges = 0;             			// initialize counter

	
		GPIO_PORTJ_IS_R = 0;     						// (Step 1) PJ1 is Edge-sensitive 
		GPIO_PORTJ_IBE_R = 0;    						//     			PJ1 is not triggered by both edges 
		GPIO_PORTJ_IEV_R = 0;    						//     			PJ1 is falling edge event 
		GPIO_PORTJ_ICR_R = 0x03;      			// 					Clear interrupt flag by setting proper bit in ICR register
		GPIO_PORTJ_IM_R = 0x03;      				// 					Arm interrupt on PJ1 by setting proper bit in IM register
    
		NVIC_EN1_R = 0x00080000;            // (Step 2) Enable interrupt 51 in NVIC (which is in Register EN1)
	
		NVIC_PRI12_R = 0xA0000000; 					// (Step 4) Set interrupt priority to 5

		EnableInt();           							// (Step 3) Enable Global Interrupt. lets go!
}

// Interrupt initialization for GPIO Port J IRQ# 51
/*void PortJ1_Interrupt_Init(void){
	
		FallingEdges = 0;             			// initialize counter

	
		GPIO_PORTJ_IS_R = 0;     						// (Step 1) PJ1 is Edge-sensitive 
		GPIO_PORTJ_IBE_R = 0;    						//     			PJ1 is not triggered by both edges 
		GPIO_PORTJ_IEV_R = 0;    						//     			PJ1 is falling edge event 
		GPIO_PORTJ_ICR_R = 0x02;      			// 					Clear interrupt flag by setting proper bit in ICR register
		GPIO_PORTJ_IM_R = 0x02;      				// 					Arm interrupt on PJ1 by setting proper bit in IM register
    
		NVIC_EN1_R = 0x00080000;            // (Step 2) Enable interrupt 51 in NVIC (which is in Register EN1)
	
		NVIC_PRI12_R = 0xA0000000; 					// (Step 4) Set interrupt priority to 5

		EnableInt();           							// (Step 3) Enable Global Interrupt. lets go!
}
*/



//	(Step 5) IRQ Handler (Interrupt Service Routine).  
//  				This must be included and match interrupt naming convention
//	 				in startup_msp432e401y_uvision.s 
//					(Note - not the same as Valvano textbook).
/*void GPIOJ1_IRQHandler(void){
  FallingEdges = FallingEdges + 1;	// Increase the global counter variable ;Observe in Debug Watch Window
	FlashLED2(1);	
	UART_OutString("\n\n400407170\r\n");												//Flash the LED D2 one time
	GPIO_PORTJ_ICR_R = 0x02;     					// Acknowledge flag by setting proper bit in ICR register
}*/
void GPIOJ_IRQHandler(void){
  FallingEdges = FallingEdges + 1;	// Increase the global counter variable ;Observe in Debug Watch Window
	if ((GPIO_PORTJ_DATA_R & 0b00000010) == 0){
        UART_OutString("400407170");
     }
      if ((GPIO_PORTJ_DATA_R & 0b00000001) == 0){
        UART_OutString("2dx3");
      }
										//Flash the LED D2 one time
													//Flash the LED D2 one time
	GPIO_PORTJ_ICR_R = 0x03;     					// Acknowledge flag by setting proper bit in ICR register
}


//	(Step 6) The main program -- 
//		notice how we are only initializing the micro and nothing else.
// 		Our configured interrupts are being handled and tasks executed on an event driven basis.
int main(void){
  PLL_Init();           // Set system clock to 120 MHz
	SysTick_Init();
	UART_Init();
	PortN_Init();							// Initialize the onboard push button on PJ1
	PortJ_Init();							// Initialize the onboard push button on PJ1
	PortJ_Interrupt_Init();		// Initalize and configure the Interrupt on Port J
	//PortJ_Interrupt_Init();		// Initalize and configure the Interrupt on Port J


	while(1){							// Inside an infinite while loop, 
		WaitForInt();				// Call WaitForInt() function to wait for interrupts
	}
}