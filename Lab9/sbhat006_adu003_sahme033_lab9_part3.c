/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Partner 3 Syed Ahmed & E-mail: sahme033@ucr.edu
* Lab section: B21
* Assignment: Lab 9 Exercise 3
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 
#include <avr/io.h>
#include "timer.h"

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

#define E5 659.26
#define Eb 622.25
#define B4 493.88
#define D5 587.33
#define C5 523.25
#define A4 440.00

const double songArray[10] = {E5, Eb, E5, Eb, E5, B4, D5, C5, A4, A4};

unsigned char tmpA;
unsigned int i = 0;

enum States {SMstart, wait, play, pause, wait_fe} state;

void ToneSM(){
	switch(state){
		case SMstart:
		state = wait;
		break;
		
		case wait:
		if(tmpA){
			state = play;
		}
		else{
            state = wait;
            set_PWM(0);}
		break;
		
		case play:
		if( i < 10 ) {
			set_PWM(songArray[i]);
			i++;
			state = pause;
			break;
		}
		else { state = wait_fe; i = 0; set_PWM(0); }
		break;
		
		case pause:
		state = play;
		break;
		
		case wait_fe:
		if(!tmpA)
		{
			state = wait;
		}
		break;
		
		default:
		state = wait;
		break;
	}
}

int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = SMstart;
	tmpA = i = 0x00;
	TimerOn();
	TimerSet(200);
	PWM_on();
	while(1)
	{
		tmpA = PINA & 0x01;
		ToneSM();
		while(!TimerFlag){};
		TimerFlag = 0;
	}
}