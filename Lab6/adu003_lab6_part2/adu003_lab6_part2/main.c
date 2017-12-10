/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab section: B21
* Assignment: Lab 6 Exercise 2
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "io.c"
#include "timer.h"

enum States { 
	START, 
	INIT, 
	PLAY, 
	PAUSE 
	} state;     
unsigned char tmpB;                     
unsigned char direction;                      
unsigned char button;                     
unsigned char wait;                    
unsigned char count;                      
unsigned char score;                    
const unsigned char period = 150;       
const unsigned char* winner = "Winner!";  
const unsigned char* loser = "Loser!"; 

void increment() {
	score = (score < 9) ? score + 1 : score;    
	LCD_Cursor(1);                              
	LCD_WriteData(score + '0');               
}

void decrement() {
	score = (score > 0) ? score - 1 : score;   
	LCD_Cursor(1);                              
	LCD_WriteData(score + '0');               
}

void Tick() {
	
	button = ~PINA & 0x01;                

	// State Transitions
	switch (state) {
		case START:
		state = INIT; 
		break;  

		case INIT:
		direction = 0;               
		wait = 0;                   
		state = PLAY; 
		break;   

		case PLAY:
		if (button) 
		{
			state = PAUSE;          
			wait = 1;           
			count = 0;               
			if (tmpB & 0x02) 
			{
				increment();        
				
				if (score == 9) 
				{
					LCD_DisplayString(1, winner);  
				}
			}
			else 
			{
				decrement();        
				if (score == 0) 
				{
					LCD_DisplayString(1, loser); 
				}
			}
		}
		count++;                     
		break;
		case PAUSE:
		
		if (!button && wait)
		{
			wait = 0;  
		}
		else if (button && !wait) 
		{
			if (score == 9 || score == 0) 
			{
				state = INIT;
			}
			else 
			{
				state = PLAY;          
			}
		}
		break;

		default:
		state = START;             
		break;
	}

	// State Actions
	switch (state) {
		case INIT:
		LCD_ClearScreen();                      
		tmpB = (tmpB & 0x00) | 0x01;          
		score = 5;                            
		LCD_Cursor(1);                          
		LCD_WriteData(score + '0'); 
		break;    

		case PLAY:
		
		if (count % 2) 
		{
			tmpB = (!direction) ? (tmpB << 1) : (tmpB >> 1);  
		}

		if (tmpB <= 0x01 && direction) 
		{
			direction = 0;   
		}
		else if (tmpB >= 0x04 && !direction) 
		{
			direction = 1;    
		}
		break;

		default:
		break;
	}

	PORTB = tmpB;               
	LCD_Cursor(0);                            
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	DDRD = 0xFF; PORTD = 0x00; 

	LCD_init();

	TimerSet(period);            
	TimerOn();                
	state = START;             

	while(1)
	{
		Tick();                
		while (!TimerFlag);     
		TimerFlag = 0;         
	}
}