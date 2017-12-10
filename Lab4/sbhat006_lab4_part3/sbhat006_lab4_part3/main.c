/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 4 Exercise 3
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>

enum SM_STATES{INIT, BUTTON_RE, BUTTON_FE}SM_STATE;

unsigned char lightPatterns[] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0xAA, 0x55, 0xAA, 0x55};

unsigned char nextPattern = 0;

unsigned char maxPattern = 9;

unsigned char tmpA;

unsigned char tmpB;

void TICK_SM()
{
	// Transitions
	switch(SM_STATE)
	{
		case INIT:
			if(tmpA)
				SM_STATE = BUTTON_RE;
			else
				SM_STATE = INIT;
			break;
			
		case BUTTON_RE:
			if(!tmpA)
				SM_STATE = BUTTON_FE;
			else
				SM_STATE = BUTTON_RE;
			break;
			
		case BUTTON_FE:
			if(tmpA)
				SM_STATE = BUTTON_RE;
			else
				SM_STATE = INIT;
			break;
			
		default:
			SM_STATE = INIT;
			break;
		
	}
	
	// State Actions
	switch(SM_STATE)
	{
		case BUTTON_RE:
			tmpB = lightPatterns[nextPattern];
		break;
		
		case BUTTON_FE:
			if(nextPattern < maxPattern)
				nextPattern++;
			else
				nextPattern = 0;
				
		default:
			break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	SM_STATE = INIT;
	tmpA = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		//PORTB = 0xFF;
		tmpA = PINA & 0x01;
		TICK_SM();
		PORTC = tmpB;
    }
}

