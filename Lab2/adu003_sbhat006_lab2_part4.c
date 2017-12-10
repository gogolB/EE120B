/* Partner 1 Souradeep Bhattacharya & E-mail:sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail:adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 2 Exercise 4
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x0F; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	DDRC = 0xF0; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	// initialize to 0s
	unsigned char ValA = 0x00; // intermediate variable used for port updates
	unsigned char ValB = 0x00;
	unsigned char ValC = 0x00;
	while(1)
	{
		// 1) Read Inputs and assign to variables
		ValA = PINA & 0xFF; // Mask PINA to only get the bit you are interested in
		// 2) Perform Computation
		
		ValB = ValA & 0xF0;
		ValB = ValB >> 4;
		
		ValC = ValA & 0x0F;
		ValC = ValC << 4;
		
		// 3) write results to port
		PORTB = ValB;
		PORTC = ValC;
	}
}


