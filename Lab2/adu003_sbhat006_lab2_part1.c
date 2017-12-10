/* Partner 1 Souradeep Bhattacharya & E-mail:sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail:adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 2 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as outputs,
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
    // initialize to 0s
    unsigned char valA = 0x00; // intermediate variable used for port updates
	unsigned char valB = 0x00;
    unsigned char cnt = 0x00;
    while(1)
    {
	    // 1) Read Inputs and assign to variables
	    valA = PINA & 0xFF; // Mask PINA to only get the bit you are interested in
		valB = PINB & 0xFF; // Mask PINA to only get the bit you are interested in
	    // 2) Perform Computation
		cnt = 0;
		unsigned char bitmask = 1;
		for(char i = 0; i < sizeof(unsigned char) * 8; i++)
		{
			if((valA & bitmask) != 0)
				cnt++;
			if((valB & bitmask) != 0)
				cnt++;
			bitmask = bitmask << 1;
		}
	    // 3) write results to port
	    PORTC = cnt;
	}
}

