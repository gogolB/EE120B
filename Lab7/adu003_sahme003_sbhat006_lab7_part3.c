/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Partner 3 Syed Ahmed & E-mail: sahme033@ucr.edu
* Lab section: B21
* Assignment: Lab 7 Exercise 3
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

enum SM_3LED{LED1, LED2, LED3} STATE_3LED;
enum SM_BLINK{LED_ON, LED_OFF} STATE_BLINK;
enum SM_BLINK{SOUND_ON, SOUND_OFF, SOUND_WAIT} STATE_SOUND;

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char sound;
unsigned char outputLED;

unsigned short count_3LED;
unsigned short count_BlinkLED;
unsigned short count_Sound;

unsigned short transitionTime_3LED = 300;
unsigned short transitionTIme_BlinkLED = 1000;
unsigned char transitionTime_sound = 2;

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

void TICK_3LED()
{
	// transitions
	switch(STATE_3LED)
	{
		case LED1:
			if(count_3LED > transitionTime_3LED)
			{
				STATE_3LED = LED2;
				count_3LED = 0;
			}
		break;
		
		case LED2:
			if(count_3LED > transitionTime_3LED)
			{
				STATE_3LED = LED3;
				count_3LED = 0;
				
			}
		break;
		
		case LED3:
			if(count_3LED > transitionTime_3LED)
			{
				STATE_3LED = LED1;
				count_3LED = 0;
			}
		break;
		
	}
	
	// Actions
	switch(STATE_3LED)
	{
		case LED1:
			threeLEDs = 0x01;
			count_3LED++;
		break;
		
		case LED2:
			threeLEDs = 0x02;
			count_3LED++;
		break;
		
		case LED3:
			threeLEDs = 0x04;
			count_3LED++;
		break;
	}
	
}

void TICK_BLINK()
{
	// Transitions
	switch(STATE_BLINK)
	{
		case LED_ON:
			if(count_BlinkLED > transitionTIme_BlinkLED)
			{
				STATE_BLINK = LED_OFF;
				count_BlinkLED = 0;
			}
		break;
		
		case LED_OFF;
			if(count_BlinkLED > transitionTIme_BlinkLED)
			{
				STATE_BLINK = LED_ON;
				count_BlinkLED = 0;
			}
		break;
		
	}
	
	// Actions
	switch(STATE_BLINK)
	{
		case LED_ON:
			blinkingLED = 0x04;
			count_BlinkLED++;
		break;
		
		case LED_OFF:
			blinkingLED = 0x00;
			count_BlinkLED++;
		break;
	}
}

void TICK_SOUND()
{
	// Transitions
	switch(STATE_SOUND)
	{
		case SOUND_WAIT:
			if((PINA & 0x08) == 0x08)
			{
				STATE_SOUND = SOUND_ON;
				count_Sound = 0;
			}
		break;
		
		case SOUND_ON:
			if((PINA & 0x08) == 0x0)
			{
				STATE_SOUND = SOUND_WAIT;
				count_Sound = 0;
			}
			else if(count_Sound > transitionTime_sound)
			{
				STATE_SOUND = SOUND_OFF;
				count_Sound = 0;
			}
		break;
		
		case SOUND_OFF:
			if((PINA & 0x08) == 0x0)
			{
				STATE_SOUND = SOUND_WAIT;
				count_Sound = 0;
			}
			else if(count_Sound > transitionTime_sound)
			{
				STATE_SOUND = SOUND_ON;
				count_Sound = 0;
			}
		break;		
		
	}
	// Action
	switch(STATE_SOUND)
	{
		case SOUND_WAIT:
			sound = 0x00;
		break;
		
		case SOUND_ON:
			sound = 0x50;
			count_Sound++;
		break;
		
		case SOUND_OFF:
			sound = 0x00;
			count_Sound++;
		break;
	}
}

void TICK_COMBINE_LED()
{
	PORTB = threeLEDs|blinkingLED|sound;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	TimerSet(1);             // set Timer period
	TimerOn();                  // enable timer
	
	STATE_3LED = LED1;
	STATE_BLINK = LED_OFF;
	STATE_SOUND = SOUND_WAIT;
	
    /* Replace with your application code */
    while (1) 
    {
		TICK_3LED();
		TICK_BLINK();
		TICK_SOUND();
		TICK_COMBINE_LED();
		while (!TimerFlag);     // Wait
		TimerFlag = 0;          // reset timer flag
    }
}

