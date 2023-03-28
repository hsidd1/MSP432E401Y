#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"



void PortE0E1E2E3_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // activate the clock for Port E
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0) {}; // allow time for clock to stabilize
	GPIO_PORTE_DEN_R = 0b00001111; // Enable PE0:PE3 
	return;
}

void PortM0M1M2M3_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11; //activate the clock for Port M
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R11) == 0) {}; //allow time for clock to stabilize
	GPIO_PORTM_DIR_R = 0b00000000; // Make PM0:PM3 inputs, reading if the button is pressed or not
	GPIO_PORTM_DEN_R = 0b00001111; // Enable PM0:PM3
	return;
}

//Turns on D2, D1
void PortN0N1_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12; //activate the clock for Port N
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R12) == 0) {};//allow time for clock to stabilize
	GPIO_PORTN_DIR_R = 0b00000011; //Make PN0 and PN1 outputs, to turn on LED's
	GPIO_PORTN_DEN_R = 0b00000011; //Enable PN0 and PN1
	return;
}

void PortL_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R10) == 0) {}; //allow time for clock to stabilize
	GPIO_PORTL_DIR_R = 0xF;
}

void PortH_Init(void){
	//Use PortM pins for output
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;				// activate clock for Port N
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R7) == 0){};	// allow time for clock to stabilize
	GPIO_PORTH_DIR_R |= 0xFF;        								// make PN0 out (PN0 built-in LED1)
  GPIO_PORTH_AFSEL_R &= ~0xFF;     								// disable alt funct on PN0
  GPIO_PORTH_DEN_R |= 0xFF;        								// enable digital I/O on PN0
																									// configure PN1 as GPIO
  //GPIO_PORTM_PCTL_R = (GPIO_PORTM_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTH_AMSEL_R &= ~0xFF;     								// disable analog functionality on PN0		
	return;
}


//Turns on D3, D4
void PortF0F4_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; //activate the clock for Port F
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {};//allow time for clock to stabilize
	GPIO_PORTF_DIR_R = 0b00010001; //Make PF0 and PF4 outputs, to turn on LED's
	GPIO_PORTF_DEN_R = 0b00010001;
	return;
}
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
	UART0_LCRH_R = 0x0070;								// 8-bit word length, enable FIFO   

	UART0_CTL_R = 0x0301;			// enable RXE, TXE and UART   
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011; // UART   
	GPIO_PORTA_AMSEL_R &= ~0x03;	// disable analog function on PA1-0   
	GPIO_PORTA_AFSEL_R |= 0x03;		// enable alt funct on PA1-0   
	GPIO_PORTA_DEN_R |= 0x03;			// enable digital I/O on PA1-0 
}
	void UART_OutChar(char data){
		while((UART0_FR_R&0x0020) != 0);	// wait until TXFF is 0   
		UART0_DR_R = data;
	} 
	
void UART_OutString(char* data){
		int i=0;
		while(data[i]!=0){
      UART_OutChar(data[i]);  // add one line of code to complete this function
      i = i+1;
		}
	}

void FlashLED3(int count){
	for (int i = 0; i < count; i++) {
	GPIO_PORTN_DATA_R ^= 0b10;
	SysTick_Wait10ms(2);
	GPIO_PORTN_DATA_R ^= 0b10;
	SysTick_Wait10ms(2);
	}
}
int steps = 0;
int totalsteps = 0;

void spinhome(int cw, int step){
	int delay = 200000;
	int angle;
	int degr;
	char* deg;
	char* direction;
	if(step){
		angle = 16;
		degr = 1;
	}
	else{
	angle = 64;
		degr = 0;
	}
	if (!cw){
		if (degr){
			direction = "\n\nSpinning Counterclockwise: 11.25 degrees\r\n";
		}
		else{ direction = "\n\nSpinning Counterclockwise: 45 degrees\r\n";
		}
		UART_OutString(direction);
		for (int i = 0; i < step; i++){
		GPIO_PORTH_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		steps++;
		totalsteps--;
	}
		if(steps == 64){
		FlashLED3(3);
		steps = 0;
		}
	}
	if (cw){
		if (degr){
			direction = "Spinning Clockwise: 11.25 degrees";
		}
		else{ direction = "Spinning Clockwise: 45 degrees";
		}
		UART_OutString(direction);
		for (int j = 0; j <step; j++){
		GPIO_PORTH_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		steps++;
		totalsteps++;
		}
		if(steps == 64){
		FlashLED3(3);
		steps = 0;
		}
		
	}
}
void spin(int cw, int step){
	int delay = 200000;
	int angle;
	int degr;
	char* deg;
	char* direction;
	if(step){
		angle = 16;
		degr = 1;
	}
	else{
	angle = 64;
		degr = 0;
	}
	if (!cw){
				if (degr){
			direction = "\n\nSpinning Counterclockwise: 11.25 degrees\r";
		}
		else{ direction = "\n\nSpinning Counterclockwise: 45 degrees\r";
		}
		UART_OutString(direction);
		for (int i = 0; i < angle; i++){
		GPIO_PORTH_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		steps++;
		totalsteps--;
	}
		if(steps == 64){
		FlashLED3(3);
		steps = 0;
		}
	}
	if (cw){
				if (degr){
			direction = "\n\nSpinning Clockwise: 11.25 degrees\r";
		}
		else{ direction = "\n\nSpinning Clockwise: 45 degrees\r";
		}
		UART_OutString(direction);
		for (int j = 0; j <angle; j++){
		GPIO_PORTH_DATA_R = 0b00001001;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000011;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00000110;
		SysTick_Wait(delay);
		GPIO_PORTH_DATA_R = 0b00001100;
		SysTick_Wait(delay);
		steps++;
		totalsteps++;
		}
		if(steps == 64){
		FlashLED3(3);
		steps = 0;
		}
		
	}
}

void Button0(void){
	GPIO_PORTF_DATA_R ^= 0b1;
}

void Button1(void){
	GPIO_PORTF_DATA_R ^= 0b010000;
}

void Button2(void){
	GPIO_PORTN_DATA_R ^= 0b1;
}

void Button3(void){
	//GPIO_PORTN_DATA_R ^= 0b10;
}


	
uint8_t key;
uint8_t output;
uint8_t input;
uint8_t input_output;
uint8_t GPIOPORTM;
uint8_t GPIOPORTE;


int main(void) {
	PortE0E1E2E3_Init();
	PortM0M1M2M3_Init();
	PortF0F4_Init();
	PortN0N1_Init();
	PortH_Init();
	SysTick_Init();
	UART_Init();

	//Turns off all LEDs
	GPIO_PORTF_DATA_R = 0b010000;
	GPIO_PORTN_DATA_R = 0b00000001;

	while (1) {
		//Row 1
		GPIO_PORTE_DIR_R = 0b00000001; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;
		GPIOPORTE = GPIO_PORTE_DATA_R;
		GPIOPORTM = GPIO_PORTM_DATA_R;

		//Column 1 - Button 1
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//1
			key = 1;
			output = 0b1110;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11101110;
			//GPIO_PORTF_DATA_R = 0b1;
			//GPIO_PORTN_DATA_R = 0b0;
			Button1();
			SysTick_Wait10ms(5);
		}

		//Column 2 - Button 2
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//2
			key = 2;
			output = 0b1110;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11011110;
			//GPIO_PORTF_DATA_R = 0b010000;
			//GPIO_PORTN_DATA_R = 0;
			Button2();
			SysTick_Wait10ms(5);
		}

		//Column 3 - Button 3
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//3
			key = 3;
			output = 0b1110;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b10111110;
			//GPIO_PORTF_DATA_R = 0b10001;
			//GPIO_PORTN_DATA_R = 0;
			Button3();
			if (totalsteps == 0){
			}
			if (totalsteps < 0){
				spinhome(1, -totalsteps);
			}
			else{
				spinhome(0, totalsteps);
			}
			SysTick_Wait10ms(5);
		}

		//Column 4 - Button 4
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//A
			key = 10;
			output = 0b1110;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b01111110;
			GPIO_PORTN_DATA_R = 0b10;
			GPIO_PORTF_DATA_R = 0b10000;
		}


		//Row 2
		GPIO_PORTE_DIR_R = 0b00000010; //Drive Row 2
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 5
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//4
			key = 4;
			output = 0b1101;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11101101;
			GPIO_PORTN_DATA_R = 0b1;
			GPIO_PORTF_DATA_R = 0;
		}

		//Column 2 - Button 6
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//5
			key = 5;
			output = 0b1101;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11011101;
			GPIO_PORTN_DATA_R = 0b1;
			GPIO_PORTF_DATA_R = 0b1;
		}

		//Column 3 - Button 7
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//6
			key = 6;
			output = 0b1101;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b10111101;
			GPIO_PORTN_DATA_R = 0b1;
			GPIO_PORTF_DATA_R = 0b10000;
		}

		//Column 4 - Button 
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//B
			key = 11;
			output = 0b1101;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b01111101;
			GPIO_PORTN_DATA_R = 0b10;
			GPIO_PORTF_DATA_R = 0b10001;
		}


		//Row 3
		GPIO_PORTE_DIR_R = 0b00000100; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 9
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//7
			key = 7;
			output = 0b1011;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11101011;
			GPIO_PORTN_DATA_R = 0b1;
			GPIO_PORTF_DATA_R = 0b10001;
		}

		//Column 2 - Button 10
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//8
			key = 8;
			output = 0b1011;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11011011;
			GPIO_PORTN_DATA_R = 0b10;
			GPIO_PORTF_DATA_R = 0;
		}

		//Column 3 - Button 11
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//9
			key = 9;
			output = 0b1011;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b10111011;
			GPIO_PORTN_DATA_R = 0b10;
			GPIO_PORTF_DATA_R = 0b1;
		}

		//Column 4 - Button 12
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//C
			key = 12;
			output = 0b1011;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b01111011;
			GPIO_PORTN_DATA_R = 0b11;
			GPIO_PORTF_DATA_R = 0;
		}


		//Row 4
		GPIO_PORTE_DIR_R = 0b00001000; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 13
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//*
			key = 14;
			output = 0b0111;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11100111;
		}

		//Column 2 - Button 14
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//0
			key = 0;
			output = 0b0111;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b11010111;
			//GPIO_PORTN_DATA_R = 0;
			//GPIO_PORTF_DATA_R = 0;
			Button0();
			SysTick_Wait10ms(5);
		}

		//Column 3 - Button 15
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//#
			key = 15;
			output = 0b0111;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b10110111;
		}

		//Column 4 - Button 16
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//D
			key = 13;
			output = 0b0111;
			input = GPIO_PORTM_DATA_R;
			input_output = 0b01110111;
			GPIO_PORTN_DATA_R = 0b11;
			GPIO_PORTF_DATA_R = 0b1;
		}
		uint8_t gpioportf = GPIO_PORTF_DATA_R;
		if((GPIO_PORTF_DATA_R & 0b1) == 1){
			spin(GPIO_PORTF_DATA_R & 0b010000, GPIO_PORTN_DATA_R & 0b1);
			GPIO_PORTF_DATA_R &= ~0b1;
		}
	}
}