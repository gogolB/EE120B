/*
 * part3.c
 *
 * Created: 8/21/2017 8:11:41 PM
 * Author : Gogol
 */ 

#include <avr/io.h>



void test()
{
	PORTC = 0xF0;
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Replace with your application code */
	PWM_on();
    while (1) 
    {
		set_PWM(440);
    }
}

