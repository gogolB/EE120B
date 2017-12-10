/*
 * lab9Part1.c
 *
 * Created: 8/21/2017 7:52:36 PM
 * Author : Gogol
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
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

static unsigned char tmpA;
static unsigned char buttonA;
static unsigned char buttonB;
static unsigned char buttonC;

static enum SM_Frequency{WAIT_FOR_BUTTON, PLAY_NOTE_C, PLAY_NOTE_D, PLAY_NOTE_E}STATE;

void SM_Tick()
{
	tmpA = PINA & 0xFF;
	buttonA = ((tmpA & 0x01) == 0x01);
	buttonB = ((tmpA & 0x02) == 0x02);
	buttonC = ((tmpA & 0x04) == 0x04);
	
	// Transition
	switch(STATE)
	{
		case WAIT_FOR_BUTTON:
		//set_PWM(0);
		if(buttonA && !buttonB && !buttonC)
		{
			STATE = PLAY_NOTE_C;
			//set_PWM(261.63);
		}
		if(!buttonA && buttonB && !buttonC)
		{
			STATE = PLAY_NOTE_D;
			//set_PWM(293.66);
		}
		if(!buttonA && !buttonB && buttonC)
		{
			STATE = PLAY_NOTE_E;
			//set_PWM(329.63);
		}
		if(!buttonA && !buttonB && !buttonC)
		{
			set_PWM(0);
		}
		break;
		
		case PLAY_NOTE_C:
		if(buttonA && !buttonB && !buttonC)
		{
			STATE = PLAY_NOTE_C;
			set_PWM(261.63);
			break;
		}
		else
		{
			STATE = WAIT_FOR_BUTTON;
			set_PWM(0);
		}
		break;
		
		case PLAY_NOTE_D:
		if(!buttonA && buttonB && !buttonC)
		{
			STATE = PLAY_NOTE_D;
			set_PWM(293.66);
		}
		else
		{
			STATE = WAIT_FOR_BUTTON;
			set_PWM(0);
		}
		break;
		
		case PLAY_NOTE_E:
		if(!buttonA && !buttonB && buttonC)
		{
			STATE = PLAY_NOTE_E;
			set_PWM(329.63);
		}
		else
		{
			STATE = WAIT_FOR_BUTTON;
			set_PWM(0);
		}
		break;
	}

}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	STATE = WAIT_FOR_BUTTON;
	
	PWM_on();
	set_PWM(5000);
    /* Replace with your application code */
    while (1) 
    {
		SM_Tick();
    }
}

