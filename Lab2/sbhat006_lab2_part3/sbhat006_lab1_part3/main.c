/*
 * sbhat006_lab1_part3.c
 *
 * Created: 7/31/2017 9:21:53 PM
 * Author : sbhattacharya
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char amtFuel = 0x00; // intermediate variable used for port updates
	unsigned char outputLights = 0x00;
	unsigned char keyInIgnition = 0x00;
	unsigned char seatedDriver = 0x00;
	unsigned char seatBeltFastened = 0x00;
	while(1)
	{
		// 1) Read Inputs and assign to variables
		amtFuel = PINA & 0x0F; // Mask PINA to only get the bit you are interested in
		keyInIgnition = PINA & (1 << 4);
		seatedDriver = PINA & (1 << 5);
		seatBeltFastened = PINA & (1 << 6);
		
		// Calc
		if(amtFuel == 1 || amtFuel ==2)
			outputLights = 0x20;
		else if(amtFuel == 3 || amtFuel == 4)
			outputLights = 0x30;
		else if(amtFuel == 5 || amtFuel == 6)
			outputLights = 0x38;
		else if(amtFuel == 7 || amtFuel == 8 || amtFuel == 9)
			outputLights = 0x3C;
		else if(amtFuel == 10 || amtFuel == 11 || amtFuel == 12)
			outputLights = 0x3E;
		else if(amtFuel == 13 || amtFuel == 14 || amtFuel == 15)
			outputLights = 0x3F;
		
		if(amtFuel <= 4)
			outputLights = outputLights |(1 << 6);
		
		if(keyInIgnition != 0 && seatedDriver != 0 && !seatBeltFastened)
			outputLights = outputLights |(1 << 7);
		
		// 3) write results to port
		PORTC = outputLights;
	}
}


