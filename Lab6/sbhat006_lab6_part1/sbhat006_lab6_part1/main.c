/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab section: B21
* Assignment: Lab 6 Exercise 1
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "io.c"

/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 4 Exercise 2
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>

enum SM_STATES{SM_WAIT_FOR_BUTTON, SM_WAIT_FOR_INC_FALL, SM_WAIT_FOR_DEC_FALL, SM_WAIT_BOTH_FALL} SM_STATE;

unsigned char A = 0x00;
unsigned char C = 0x00;

#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

unsigned char tmpA = 0x00;

unsigned char lightPattherns[] = {0x01, 0x02, 0x04, 0x02};

unsigned char maxPatterns = 4;

unsigned char nextPattern = 0;

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


void SM_Tick()
{
	// Transition Tree
	switch(SM_STATE)
	{
		case SM_WAIT_BOTH_FALL:
		if(A == 0x00)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else
		SM_STATE = SM_WAIT_BOTH_FALL;
		break;
		
		case SM_WAIT_FOR_BUTTON:
		if(A == 0x03)
		{
			C = 0;
			SM_STATE = SM_WAIT_FOR_BUTTON;
		}
		if(A == 0x01)
		{
			if(C < 9)
			C++;
			SM_STATE = SM_WAIT_FOR_BUTTON;
		}
		else if(A == 0x02)
		{
			if(C > 0)
			C--;
			SM_STATE = SM_WAIT_FOR_BUTTON;
		}
		break;
		
		case SM_WAIT_FOR_INC_FALL:
		if(A == 0x00 || A == 0x02)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else if(A == 0x03)
		{
			C = 0;
			SM_STATE = SM_WAIT_BOTH_FALL;
		}
		break;
		
		case SM_WAIT_FOR_DEC_FALL:
		if(A == 0x00 || A == 0x01)
		SM_STATE = SM_WAIT_FOR_BUTTON;
		else if(A == 0x03)
		{
			C = 0;
			SM_STATE = SM_WAIT_BOTH_FALL;
		}
		break;
	}
}

int main(void)
{
	DDRC=0xFF; PORTC = 0x00;
	DDRD=0xFF; PORTD = 0x00;
	DDRA=0x00; PORTA = 0xFF;
    
	
	LCD_init();
	
	//LCD_DisplayString(1, "HELLO WORLD");
	
	// Init.
	SM_STATE = SM_WAIT_FOR_BUTTON;
	C = 0x07;
	PORTB = C;

	TimerSet(1000);             // set Timer period
	TimerOn();                  // enable timer
	
	
    while (1) 
    {
		A = PINA & 0x03;
		SM_Tick();
		LCD_DisplayString(2, " 0xFACEBOOC");
		LCD_Cursor(1);
		LCD_WriteData(C + '0');
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

