/*
 * sbhat006_lab9_part3.c
 *
 * Created: 8/21/2017 10:59:23 AM
 * Author : Gogol
 */ 

#include <avr/io.h>

unsigned char playButton = 0x00;

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
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

// *****************************************************************************************
//										PWM functions
// *****************************************************************************************

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

// *****************************************************************************************
//										Internal functions
// *****************************************************************************************
enum NOTES{C4,D4,E4,F4,G4,A4,B4,C5,S};
enum WAITS{EIGHT,QUARTER,HALF,WHOLE};

double getFrq(enum NOTES N)
{
	switch(N)
	{
		case C4:
			return 261.63;
		case D4:
			return 296.66;
		case E4:
			return 329.63;
		case F4:
			return 349.23;
		case G4:
			return 392.00;
		case A4:
			return 440.00;
		case B4:
			return 493.88;
		case C5:
			return 523.25;
		
		default:
		case S:
			return 0;
	}
}

unsigned short getWaits(enum WAITS W)
{
	switch(W)
	{
		case EIGHT:
			return 125;
		case QUARTER:
			return 250;
		case HALF:
			return 500;
		case WHOLE:
			return 1000;
			
		default:
			return 0;
	}
}

unsigned short WaitFor = 0;
unsigned short currentCount = 0;

enum NOTES song[] = {C5, 
					E4, C4, A4, S, 
					F4, G4, B4,
					E4,F4,G4,
					A4,B4,C5,C4};
					
enum WAITS timeing[] = {EIGHT,
						QUARTER, EIGHT, HALF, EIGHT, 
						 HALF, QUARTER, QUARTER,
						 HALF, HALF,
						 QUARTER, QUARTER, QUARTER, QUARTER};

unsigned char ptr = 0;
unsigned char maxSongLength = 13;
						 
// *****************************************************************************************
//										State Machine functions
// *****************************************************************************************

enum SM_STATE{WAIT_FOR_BUTTON, WAIT_FOR_BUTTON_FE, PLAYING_SONG}STATE;
enum SM_STATE_SONG{WAIT_FOR_START, START,PLAY_NOTE, NEXT_NOTE}SONG_STATE;
unsigned char songEnded = 0;

void STATE_TICK()
{
	// transition
	switch(STATE)
	{
		case WAIT_FOR_BUTTON:
			if(playButton)
			{
				STATE = PLAYING_SONG;
				SONG_STATE = START;
			}
		break;
		
		case PLAYING_SONG:
			if(songEnded)
			{
				STATE = WAIT_FOR_BUTTON_FE;
			}
		break;
		
		case WAIT_FOR_BUTTON_FE:
			if(!playButton)
			{
				STATE = WAIT_FOR_BUTTON;
			}
		break;
		
		default:
			STATE = WAIT_FOR_BUTTON;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	STATE = WAIT_FOR_BUTTON;
	SONG_STATE = WAIT_FOR_START;
	
	TimerSet(1);
	TimerOn();
	
    /* Replace with your application code */
    while (1) 
    {
		playButton = ((PINA & 0x01) == 0x01);
		STATE_TICK();
		//SM_SONG_TICK();
		PORTC = STATE;
		while(!TimerFlag);
		TimerFlag=0;
    }
}

