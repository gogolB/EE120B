/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Partner 3 Syed Ahmed & E-mail: sahme033@ucr.edu
* Lab section: B21
* Assignment: Lab 7 Exercise 5
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

unsigned short timeToWaitBeforeFastDecrement = 3000;	// 15 * 200ms = 3s
unsigned short timeToWait = 1000;						// 5 * 200ms = 1s

unsigned char count = 7;

unsigned char maxCount = 9;
unsigned char minCount = 0;

unsigned short wait_counter = 0;
unsigned short wait_wait_counter = 0;
enum SM_COUNTER{COUNTER_WAIT, COUNTER_INC_DOWN, COUNTER_DEC_DOWN, COUNTER_RESET} STATE_COUNTER;

enum SM_WAIT_CONTROLLER{WAIT_WAIT, WAIT_BUTTON_DOWN, WAIT_DECREASE, WAIT_RESET} STATE_WAIT_CONTROLLER;

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
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char INC_DOWN = 0;
unsigned char DEC_DOWN = 0;

void TICK_COUNTER()
{
	// Get the input states
	unsigned char tmpA = PINA & 0xFF;
	INC_DOWN  = (tmpA & 0x01) == 0x01;
	DEC_DOWN = (tmpA & 0x02) == 0x02;
	
	// Transitions
	switch(STATE_COUNTER)
	{
		case COUNTER_WAIT:
			if(INC_DOWN && DEC_DOWN)
			{
				STATE_COUNTER = COUNTER_RESET;
			}
			else if(INC_DOWN)
			{
				if(count < maxCount)
					count++;
				STATE_COUNTER = COUNTER_INC_DOWN;
			}
			else if(DEC_DOWN)
			{
				if(count > minCount)
					count--;
				STATE_COUNTER = COUNTER_DEC_DOWN;
			}
		break;
		
		case  COUNTER_RESET:
			if(INC_DOWN && DEC_DOWN)
			{
				STATE_COUNTER = COUNTER_RESET;
				
			}
			else
			{
				STATE_COUNTER = COUNTER_WAIT;
			}
		break;
		
		case COUNTER_INC_DOWN:
			if(INC_DOWN && DEC_DOWN)
			{
				STATE_COUNTER = COUNTER_RESET;
				wait_counter = 0;
				count = 0;
			}
			else if(!INC_DOWN || wait_counter > timeToWait)
			{
				STATE_COUNTER = COUNTER_WAIT;
				wait_counter = 0;
			}
			else
			{
				wait_counter++;
			}
		break;
		
		case COUNTER_DEC_DOWN:
			if(INC_DOWN && DEC_DOWN)
			{
				STATE_COUNTER = COUNTER_RESET;
				wait_counter = 0;
				count = 0;
			}
			if(!DEC_DOWN || wait_counter > timeToWait)
			{
				STATE_COUNTER = COUNTER_WAIT;
				wait_counter = 0;
			}
			else
			{
				wait_counter++;
			}
		break;
	}
	
}

void TICK_WAIT_CONTROLLER()
{
	switch(STATE_WAIT_CONTROLLER)
	{
		case WAIT_WAIT:
			if(INC_DOWN || DEC_DOWN)
			{
				STATE_WAIT_CONTROLLER = WAIT_BUTTON_DOWN;
				wait_wait_counter++;
			}
		break;
		
		case WAIT_BUTTON_DOWN:
			if(wait_wait_counter > timeToWaitBeforeFastDecrement)
			{
				STATE_WAIT_CONTROLLER = WAIT_DECREASE;
			}
			else if(!INC_DOWN && !DEC_DOWN)
			{
				STATE_WAIT_CONTROLLER = WAIT_RESET;
			}
			else
			{
				wait_wait_counter++;
			}
		break;
		
		case WAIT_RESET:
			STATE_WAIT_CONTROLLER = WAIT_WAIT;
		break;
		
		case WAIT_DECREASE:
			STATE_WAIT_CONTROLLER = WAIT_WAIT;
		break;
	}
	
	switch(STATE_WAIT_CONTROLLER)
	{
		case WAIT_DECREASE:
			timeToWait = 400;
			wait_wait_counter = 0;
		break;
		
		case WAIT_RESET:
			timeToWait = 1000;
			wait_wait_counter = 0;
		break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	TimerSet(1);             // set Timer period
	TimerOn();                  // enable timer
		
	STATE_COUNTER = COUNTER_WAIT;
	STATE_WAIT_CONTROLLER = WAIT_WAIT;
	
	wait_counter = 0;
	wait_wait_counter = 0;
		
    /* Replace with your application code */
    while (1) 
    {	
		TICK_COUNTER();
		TICK_WAIT_CONTROLLER();
		
		PORTB = count;
		while (!TimerFlag);     // Wait
		TimerFlag = 0;          // reset timer flag
    }
}

