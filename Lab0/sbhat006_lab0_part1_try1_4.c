/* Partner 1 Souradeep Bhattacharya & E-mail: sbhat006@ucr.edu
* Partner 2 Annie Du & E-mail: adu003@ucr.edu
* Lab Section: B21
* Assignment: Lab 0 Exercise 1
* Exercise Description: Try 1-4
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include<stdio.h>

int main()
{
	float fahr, celsius;
	int lower, upper, step;
	lower = 0;
	upper = 300;
	step = 20;
	celsius = lower;

	printf("F\tC\n", );
	while(fahr <= upper)
	{
		fahr = (9.0/5.0) * (celsius - 32.0);
		printf("%3.0f\t%6.1f\n", celsius, fahr);
		celsius = celsius + step;
	}
}