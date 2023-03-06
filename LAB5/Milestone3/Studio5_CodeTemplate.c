
// Description of Code: LED output of 4x4 keypad



#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "Systick.h"
#include "PLL.h"



void PortE0E1E2E3_Init(void) {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // activate the clock for Port E
	while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4) == 0) {}; // allow time for clock to stabilize
	GPIO_PORTE_DIR_R = 0b00001111;
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

uint8_t keytoBinarycode(uint8_t key) {
	//Row 1 or 2 or 3 or 4
	if ((key & 0b00001111) == 0b00001110) {
		if ((key & 0b11110000) == 0b11100000) {
			return 0b0001;	//1
		}
		else if ((key & 0b11110000) == 0b11010000) {
			return 0b0010;	//2
		}
		else if ((key & 0b11110000) == 0b10110000) {
			return 0b0011;	//3
		}
		else if ((key & 0b11110000) == 0b01110000) {
			return 0b1010;	//A
		}
	}
	else if ((key & 0b00001111) == 0b00001101) {
		if ((key & 0b11110000) == 0b11100000) {
			return 0b0100;	//4
		}
		else if ((key & 0b11110000) == 0b11010000) {
			return 0b0101;	//5
		}
		else if ((key & 0b11110000) == 0b10110000) {
			return 0b0110;	//6
		}
		else if ((key & 0b11110000) == 0b01110000) {
			return 0b1011;	//B
		}
	}
	else if ((key & 0b00001111) == 0b00001011) {
		if ((key & 0b11110000) == 0b11100000) {
			return 0b0111;	//7
		}
		else if ((key & 0b11110000) == 0b11010000) {
			return 0b1000;	//8
		}
		else if ((key & 0b11110000) == 0b10110000) {
			return 0b1001;	//9
		}
		else if ((key & 0b11110000) == 0b01110000) {
			return 0b1100;	//C
		}
	}
	else if ((key & 0b00001111) == 0b00000111) {
		if ((key & 0b11110000) == 0b11100000) {
			return 0b1110;	//*
		}
		else if ((key & 0b11110000) == 0b11010000) {
			return 0b0000;	//0
		}
		else if ((key & 0b11110000) == 0b10110000) {
			return 0b1111;	//#
		}
		else if ((key & 0b11110000) == 0b01110000) {
			return 0b1101;	//D
		}
	}
	return 0;
}

//Converts 4 bit binary 0bABCD to LED output 0bD1D2D3D4
void binarytoLED(uint8_t number) {
	if (number == 0b0000) {
		GPIO_PORTF_DATA_R = 0b00000000;
		GPIO_PORTN_DATA_R = 0b00000000;
	}
	else if (number == 0b0001) {
		GPIO_PORTF_DATA_R = 0b00000001;
		GPIO_PORTN_DATA_R = 0b00000000;
	}
	else if (number == 0b0010) {
		GPIO_PORTF_DATA_R = 0b00010000;
		GPIO_PORTN_DATA_R = 0b00000000;
	}
	else if (number == 0b0011) {
		GPIO_PORTF_DATA_R = 0b00010001;
		GPIO_PORTN_DATA_R = 0b00000000;
	}
	else if (number == 0b0100) {
		GPIO_PORTF_DATA_R = 0b00000000;
		GPIO_PORTN_DATA_R = 0b00000001;
	}
	else if (number == 0b0101) {
		GPIO_PORTF_DATA_R = 0b00000001;
		GPIO_PORTN_DATA_R = 0b00000001;
	}
	else if (number == 0b0110) {
		GPIO_PORTF_DATA_R = 0b00010000;
		GPIO_PORTN_DATA_R = 0b00000001;
	}
	else if (number == 0b0111) {
		GPIO_PORTF_DATA_R = 0b00010001;
		GPIO_PORTN_DATA_R = 0b00000001;
	}
	else if (number == 0b1000) {
		GPIO_PORTF_DATA_R = 0b00000000;
		GPIO_PORTN_DATA_R = 0b00000010;
	}
	else if (number == 0b1001) {
		GPIO_PORTF_DATA_R = 0b00000001;
		GPIO_PORTN_DATA_R = 0b00000010;
	}
	else if (number == 0b1010) {
		GPIO_PORTF_DATA_R = 0b00010000;
		GPIO_PORTN_DATA_R = 0b00000010;
	}
	else if (number == 0b1011) {
		GPIO_PORTF_DATA_R = 0b00010001;
		GPIO_PORTN_DATA_R = 0b00000010;
	}
	else if (number == 0b1100) {
		GPIO_PORTF_DATA_R = 0b00000000;
		GPIO_PORTN_DATA_R = 0b00000011;
	}
	else if (number == 0b1101) {
		GPIO_PORTF_DATA_R = 0b00000001;
		GPIO_PORTN_DATA_R = 0b00000011;
	}
	else if (number == 0b1110) {
		GPIO_PORTF_DATA_R = 0b00010000;
		GPIO_PORTN_DATA_R = 0b00000011;
	}
	else if (number == 0b1111) {
		GPIO_PORTF_DATA_R = 0b00010001;
		GPIO_PORTN_DATA_R = 0b00000011;
	}

	return;
}


int main(void) {
	PortE0E1E2E3_Init();
	PortM0M1M2M3_Init();
	PortF0F4_Init();
	PortN0N1_Init();

	//Turns off all LEDs
	GPIO_PORTF_DATA_R = 0b00000000;
	GPIO_PORTN_DATA_R = 0b00000000;

	uint8_t input_output;

	while (1) {
		//Row 1
		GPIO_PORTE_DATA_R = 0b11111110;

		//Column 1 - Button 1
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//1
			input_output = 0b11101110;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 2 - Button 2
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//2
			input_output = 0b11011110;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 3 - Button 3
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//3
			input_output = 0b10111110;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 4 - Button 4
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//A
			input_output = 0b01111110;
			binarytoLED(keytoBinarycode(input_output));
		}


		//Row 2
		GPIO_PORTE_DATA_R = 0b11111101;

		//Column 1 - Button 5
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//4
			input_output = 0b11101101;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 2 - Button 6
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//5
			input_output = 0b11011101;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 3 - Button 7
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//6
			input_output = 0b10111101;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 4 - Button 8
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//B
			input_output = 0b01111101;
			binarytoLED(keytoBinarycode(input_output));
		}


		//Row 3

		GPIO_PORTE_DATA_R = 0b11111011;

		//Column 1 - Button 9
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//7
			input_output = 0b11101011;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 2 - Button 10
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//8
			input_output = 0b11011011;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 3 - Button 11
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//9
			input_output = 0b10111011;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 4 - Button 12
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//C
			input_output = 0b01111011;
			binarytoLED(keytoBinarycode(input_output));
		}


		//Row 4
		GPIO_PORTE_DATA_R = 0b11110111;

		//Column 1 - Button 13
		while ((GPIO_PORTM_DATA_R & 0b00000001) == 0) {
			//*
			input_output = 0b11100111;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 2 - Button 14
		while ((GPIO_PORTM_DATA_R & 0b00000010) == 0) {
			//0
			input_output = 0b11010111;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 3 - Button 15
		while ((GPIO_PORTM_DATA_R & 0b00000100) == 0) {
			//#
			input_output = 0b10110111;
			binarytoLED(keytoBinarycode(input_output));
		}

		//Column 4 - Button 16
		while ((GPIO_PORTM_DATA_R & 0b00001000) == 0) {
			//D
			input_output = 0b01110111;
			binarytoLED(keytoBinarycode(input_output));
		}
	}
}
