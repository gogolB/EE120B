/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Partner 3 Syed Ahmed & E-mail: sahme033@ucr.edu
* Lab section: B21
* Assignment: Lab 8 Exercise 4
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>

enum States { START, INIT, WAIT } state;     
unsigned char tmpB, tmpD;                     
unsigned short myShort;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) ;
	// ADEN: setting this bit enables analog-to-digital coversion.
	// ADSC: setting this bit starts the first conversion
	// ADATE: setting this bit enables auto-triggering. Since we are
	//      in Free Running Mode, a new conversion will trigger whenever
	//      the previous conversion completes.
}

void Tick() {
	// State Transitions
	switch (state) {
		case START:
            state = INIT; 
            break;        
		case INIT:
            state = WAIT; 
            break;        
		case WAIT:
		    break;
		default:
		    state = START;              
		    break;
	}

	// State Actions
	switch (state) {
		case INIT:
		    tmpB = tmpD = 0; 
            break;   
		case WAIT:
		    myShort = ADC;          
		    tmpB = (char)myShort;   
		    if (tmpB <= 15) 
			{
				PORTB = 0x00;
            }
            else if (tmpB > 15 && tmpB <= 29) 
			{
				PORTB = 0x01;
            }
            else if (tmpB > 29 && tmpB <= 43) 
			{
				PORTB = 0x03;
            }
            else if (tmpB > 43 && tmpB <= 57) 
			{
				PORTB = 0x07;
            }
            else if (tmpB > 57 && tmpB <= 71) 
			{
				PORTB = 0x0F;
            }
            else if (tmpB > 71 && tmpB <= 85) 
			{
				PORTB = 0x1F;
            }
            else if (tmpB > 85 && tmpB <= 99) 
			{
				PORTB = 0x3F;
            }
            else if (tmpB > 85 && tmpB <= 113) 
			{
				PORTB = 0x7F;
            }
            else 
            {
                PORTB = 0xFF;
            }
            break;
		default:
		    break;
	}           
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;	

	state = START;             
	tmpB = 0;            

	ADC_init();

	while(1)
	{
		Tick();                 
	}
}