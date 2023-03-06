
// Description of Code: scanning/use of 4x4 keypad



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


//Turns on D3, D4
void PortF0F4_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; //activate the clock for Port F
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0) {};//allow time for clock to stabilize
	GPIO_PORTF_DIR_R = 0b00010001; //Make PF0 and PF4 outputs, to turn on LED's
	GPIO_PORTF_DEN_R = 0b00010001;
	return;
}


uint8_t binary_key_value;
uint8_t output;
uint8_t input;
uint8_t input_output;



int main(void) {
	PortE0E1E2E3_Init();
	PortM0M1M2M3_Init();
	PortF0F4_Init();
	PortN0N1_Init();

	//Turns off all LEDs
	GPIO_PORTF_DATA_R = 0b00000000;
	GPIO_PORTN_DATA_R = 0b00000000;

	while (1) {
		//Row 1
		GPIO_PORTE_DIR_R = 0b00000001; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 1
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//1
			binary_key_value = 1;
			output = 0b1110;
			input = 0b1110;
			input_output = 0b11101110;
		}

		//Column 2 - Button 2
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//2
			binary_key_value = 2;
			output = 0b1110;
			input = 0b1101;
			input_output = 0b11011110;
		}

		//Column 3 - Button 3
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//3
			binary_key_value = 3;
			output = 0b1110;
			input = 0b1011;
			input_output = 0b10111110;
		}

		//Column 4 - Button 4
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//A
			binary_key_value = 10;
			output = 0b1110;
			input = 0b0111;
			input_output = 0b01111110;
		}


		//Row 2
		GPIO_PORTE_DIR_R = 0b00000010; //Drive Row 2
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 5
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//4
			binary_key_value = 4;
			output = 0b1101;
			input = 0b1110;
			input_output = 0b11101101;
		}

		//Column 2 - Button 6
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//5
			binary_key_value = 5;
			output = 0b1101;
			input = 0b1101;
			input_output = 0b11011101;
		}

		//Column 3 - Button 7
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//6
			binary_key_value = 6;
			output = 0b1101;
			input = 0b1011;
			input_output = 0b10111101;
		}

		//Column 4 - Button 8
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//B
			binary_key_value = 11;
			output = 0b1101;
			input = 0b0111;
			input_output = 0b01111101;
		}


		//Row 3
		GPIO_PORTE_DIR_R = 0b00000100; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 9
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//7
			binary_key_value = 7;
			output = 0b1011;
			input = 0b1110;
			input_output = 0b11101011;
		}

		//Column 2 - Button 10
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//8
			binary_key_value = 8;
			output = 0b1011;
			input = 0b1101;
			input_output = 0b11011011;
		}

		//Column 3 - Button 11
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//9
			binary_key_value = 9;
			output = 0b1011;
			input = 0b1011;
			input_output = 0b10111011;
		}

		//Column 4 - Button 12
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//C
			binary_key_value = 12;
			output = 0b1011;
			input = 0b0111;
			input_output = 0b01111011;
		}


		//Row 4
		GPIO_PORTE_DIR_R = 0b00001000; // To drive you use the data direction register
		GPIO_PORTE_DATA_R = 0b00000000;

		//Column 1 - Button 13
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//*
			binary_key_value = 14;
			output = 0b0111;
			input = 0b1110;
			input_output = 0b11100111;
		}

		//Column 2 - Button 14
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//0
			binary_key_value = 0;
			output = 0b0111;
			input = 0b1101;
			input_output = 0b11010111;
		}

		//Column 3 - Button 15
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//#
			binary_key_value = 15;
			output = 0b0111;
			input = 0b1011;
			input_output = 0b10110111;
		}

		//Column 4 - Button 16
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//D
			binary_key_value = 13;
			output = 0b0111;
			input = 0b0111;
			input_output = 0b01110111;
		}
	}
}
