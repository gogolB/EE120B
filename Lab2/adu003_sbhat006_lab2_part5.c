/* Partner 1 Souradeep Bhattacharya & E-mail:sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail:adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 2 Exercise 5
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>


int main(void)
{
	DDRD = 0xFF; PORTD = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x01; PORTB = 0x01; // Configure port B's 8 pins as outputs,
	// initialize to 0s
	unsigned char ValD = 0x00; // intermediate variable used for port updates
	unsigned char bitD = 0x00;
	unsigned short totalWeight = 0x00;
	unsigned char airbagIndicator;
	while(1)
	{
		PORTB = 0x01;
		// 1) Read Inputs and assign to variables
		ValD = PIND & 0xFF; // Mask PINA to only get the bit you are interested in
		bitD = PINB & 0x01;
		// 2) Perform Computation
		totalWeight = (ValD << 1)| bitD;
		
		if(totalWeight >= 70)
			airbagIndicator = (1 << 1);
		else if(totalWeight > 5 && totalWeight < 70)
			airbagIndicator = (1 << 2);
		else
			airbagIndicator = 0;
		
		// 3) write results to port
		PORTB = airbagIndicator;
	}
}

