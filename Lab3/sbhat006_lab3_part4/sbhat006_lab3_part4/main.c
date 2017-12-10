/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 3 Exercise 4
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/#include <avr/io.h>

enum DI_SM_STATES{DR_SM_WAIT_FOR_INPUT, DR_SM_PROCESS_INPUT, DR_SM_WAIT_FOR_INPUT_FALL, DR_SM_UNLOCK, DR_SM_LOCK} DI_SM_STATE;

unsigned char Sequence[] = {0x04, 0x02};
unsigned char next = 0;
unsigned char maxLength = 2;
unsigned char A = 0x00;
unsigned char PA = 0x00;
unsigned char B = 0x00;

void SM_Tick()
{
	// Transition
	switch(DI_SM_STATE)
	{
		case DR_SM_WAIT_FOR_INPUT:
			PA = (A & 0x07);
			if((A & 0x80) == 0x80)
			{
				DI_SM_STATE = DR_SM_LOCK;
			}
			else if((PA & 0x01) == 0x01 || (PA & 0x02) == 0x02 || (PA & 0x04) == 0x04)
			{
				DI_SM_STATE = DR_SM_PROCESS_INPUT;
			}
			else
			{
				DI_SM_STATE = DR_SM_WAIT_FOR_INPUT;
			}
		break;
		
		case DR_SM_LOCK:
			DI_SM_STATE = DR_SM_WAIT_FOR_INPUT_FALL;
		break;
		
		case DR_SM_UNLOCK:
			DI_SM_STATE = DR_SM_WAIT_FOR_INPUT_FALL;
		break;
		
		case DR_SM_PROCESS_INPUT:
			if((A & 0x80) == 0x80)
			{
				DI_SM_STATE = DR_SM_LOCK;
				break;
			}
			DI_SM_STATE = DR_SM_WAIT_FOR_INPUT_FALL;
		break;
		
		case DR_SM_WAIT_FOR_INPUT_FALL:
			if(A == 0x00)
			{
				if(next == maxLength && B == 0x00)
				{		// Correct and Door is locked.
					DI_SM_STATE = DR_SM_UNLOCK;
					next = 0;
				}
				else if(next == maxLength && B == 0x01)	
				{	// Correct and Door is unlocked.
					DI_SM_STATE = DR_SM_LOCK;
					next = 0;
				}
				else
					DI_SM_STATE = DR_SM_WAIT_FOR_INPUT;
			}
			else
			{
				DI_SM_STATE = DR_SM_WAIT_FOR_INPUT_FALL;
			}
		break;
			
	}
	
	// State Action tree
	switch(DI_SM_STATE)
	{
		case DR_SM_LOCK:
			B = 0x00;
		break;
		
		case DR_SM_UNLOCK:
			B = 0x01;
		break;
		
		case DR_SM_PROCESS_INPUT:
			if(Sequence[next] == (A & 0x07))
				next++;
			else
				next = 0;
		break;
		
	}
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	
	// INIT
	DI_SM_STATE = DR_SM_WAIT_FOR_INPUT;
	B = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		A = PINA & 0x87;
		SM_Tick();
		PORTB = B;
    }
}



