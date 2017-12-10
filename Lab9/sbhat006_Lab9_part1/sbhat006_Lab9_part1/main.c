/*
 * sbhat006_Lab9_part1.c
 *
 * Created: 8/21/2017 10:08:47 AM
 * Author : Gogol
 */ 

#include <avr/io.h>

static double current_frequency;


void set_PWM(double frequency) {
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

// *****************************************************************************************
//										Timer functions
// *****************************************************************************************

volatile unsigned char TimerFlag = 0x00;
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
	return 0;
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	STATE = WAIT_FOR_BUTTON;
	
	//TimerSet(250);
	//TimerOn();
	PWM_on();
	
	//set_PWM(330);
    /* Replace with your application code */
    while (1) 
    {
		SM_Tick();
		PORTC = (STATE) | 1 << 3;
		//while(!TimerFlag);
		//TimerFlag = 0;
    }
	
	return 0;
}

