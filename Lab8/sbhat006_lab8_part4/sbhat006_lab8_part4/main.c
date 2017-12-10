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

unsigned short MAX = 128;
unsigned short MIN = 7;
unsigned short RANGE;

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Output
	DDRA = 0x00; PORTA = 0xFF; // Input
	
	ADC_init();
	RANGE = MAX - MIN;
	
    /* Replace with your application code */
    while (1) 
    {
		unsigned short x = ADC;
		if(x <= MIN)
			PORTB = 0x00;
		if(x > RANGE/8)
			PORTB = 0x01;
		if(x > (RANGE * 2) / 8)
			PORTB = 0x03;
		if(x > (RANGE * 3) / 8)
			PORTB = 0x07;
		if(x > (RANGE * 4) / 8)
			PORTB = 0x0F;
		if(x > (RANGE * 5) / 8)
			PORTB = 0x1F;
		if(x > (RANGE * 6) / 8)
			PORTB = 0x3F;
		if(x > (RANGE * 7) / 8)
			PORTB = 0x7F;
		if(x > MAX)
			PORTB = 0xFF;
    }
}

