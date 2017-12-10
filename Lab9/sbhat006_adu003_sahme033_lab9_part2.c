/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Partner 3 Syed Ahmed & E-mail: sahme033@ucr.edu
* Lab section: B21
* Assignment: Lab 9 Exercise 2
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 
#include <avr/io.h>

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	//set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char tmpA			= 0x00;
unsigned char buttonPlay	= 0x00;
unsigned char buttonFrqUp	= 0x00;
unsigned char buttonFrqDn	= 0x00;

const double FRQ[] = {261.93, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
int currFrq = 0;
const int MINFRQ = 0;
const int MAXFRQ = 7;

enum SM_FRQ_CONTROL{WAIT_FOR_INPUT_RE, INC_FRQ, DEC_FRQ, WAIT_FOR_INPUT_FE}FRQ_CONTROL_STATE;
enum SM_FRQ_PLAY{PLAY_FRQ, DONT_PLAY_FRQ}FRW_PLAY_STATE;

void SM_FRQ_CONTROL_TICK()
{
	switch(FRQ_CONTROL_STATE)
	{
		case WAIT_FOR_INPUT_RE:
			if(buttonFrqUp && !buttonFrqDn)
			{
				FRQ_CONTROL_STATE = INC_FRQ;
			}
			else if(!buttonFrqUp && buttonFrqDn)
			{
				FRQ_CONTROL_STATE = DEC_FRQ;
			}
			else
			{
				FRQ_CONTROL_STATE = WAIT_FOR_INPUT_RE;
			}
		break;
		
		case INC_FRQ:
			FRQ_CONTROL_STATE = WAIT_FOR_INPUT_FE;
		break;
		
		case DEC_FRQ:
			FRQ_CONTROL_STATE = WAIT_FOR_INPUT_FE;
		break;
		
		case WAIT_FOR_INPUT_FE:
			if(buttonFrqUp || buttonFrqDn)
			{
				FRQ_CONTROL_STATE = WAIT_FOR_INPUT_FE;
			}
			else
			{
				FRQ_CONTROL_STATE = WAIT_FOR_INPUT_RE;
			}
		break;
		
		default:
			FRQ_CONTROL_STATE = WAIT_FOR_INPUT_RE;
	}
	
	switch(FRQ_CONTROL_STATE)
	{
		case INC_FRQ:
			if(currFrq < 7)
				currFrq++;
			set_PWM(FRQ[currFrq]);
		break;
		
		case DEC_FRQ:
			if(currFrq > 0)
				currFrq--;
			set_PWM(FRQ[currFrq]);
		break;
		
	}
}	

void SM_FRQ_PLAY_TICK()
{
	switch(FRW_PLAY_STATE)
	{
		case PLAY_FRQ:
			if(!buttonPlay)
			{
				FRW_PLAY_STATE = DONT_PLAY_FRQ;
			}
			else
			{
				FRW_PLAY_STATE = PLAY_FRQ;
			}
		break;
		
		case DONT_PLAY_FRQ:
			if(buttonPlay)
			{
				FRW_PLAY_STATE = PLAY_FRQ;
			}
			else
			{
				FRW_PLAY_STATE = DONT_PLAY_FRQ;
			}
		break;
	}
	
	// ACTIONS
	switch(FRW_PLAY_STATE)
	{
		case PLAY_FRQ:
			PWM_on();
		break;
		
		case DONT_PLAY_FRQ:
			PWM_off();
		break; 
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	FRQ_CONTROL_STATE = WAIT_FOR_INPUT_RE;
	FRW_PLAY_STATE = DONT_PLAY_FRQ;
	
	PWM_on();
	
    /* Replace with your application code */
    while (1) 
    {
		tmpA = PINA & 0xFF;
		buttonPlay = ((tmpA & 0x01) == 0x01);
		buttonFrqUp = ((tmpA & 0x02) == 0x02);
		buttonFrqDn = ((tmpA & 0x04) == 0x04);
		SM_FRQ_CONTROL_TICK();
		SM_FRQ_PLAY_TICK();
		
    }
}

