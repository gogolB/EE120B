/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 3 Exercise 1
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>

enum LA_States{LA_B1, LA_B1_TRANS, LA_B0, LA_B0_TRANS} LA_State;

unsigned char PA;
unsigned char PB;

void TickFct_Latch()
{
	// State transition
	switch(LA_State)
	{
		case LA_B0_TRANS:
			if((PA & 1) == 0x01){
				LA_State = LA_B1;
			}
			break;
		case LA_B1:
			if((PA | 0) == 0x00){
				LA_State = LA_B1_TRANS;
			}
			break;
		case LA_B1_TRANS:
			if((PA & 1) == 0x01){
				LA_State = LA_B0;
			}
			break;
		case LA_B0:
			if((PA | 0) == 0x00){
				LA_State = LA_B0_TRANS;
			}
			break;
	}
	
	// State action
	switch(LA_State)
	{
		case LA_B0: 
			PB = 0x01;
			break;
		case LA_B1:
			PB = 0x02;
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
    // Set initial state.
	LA_State = LA_B0_TRANS;
	PB = 0x01;
	PORTB = PB;
	
    while (1) 
    {
		PA = PINA & 0xFF;
		TickFct_Latch();
		PORTB = PB;
    }
}

