/*
 * sbhat006_lab8_part1.c
 *
 * Created: 8/15/2017 11:02:40 AM
 * Author : Gogol
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

unsigned short MAX = 96;

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Output
	DDRA = 0x00; PORTA = 0xFF; // Input
	
	ADC_init();
	
    /* Replace with your application code */
    while (1) 
    {
		unsigned short x = ADC;
		
		if(x >= MAX/2)
		{
			PORTB = 0x01;
		}
		else
		{
			PORTB = 0x00;
		}
		
    }
}

