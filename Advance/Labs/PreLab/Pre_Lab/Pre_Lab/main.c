/*
* Pre_Lab.c
*
* Created: 27-09-2023 16:14:37
* Author : svcha
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

#define F_CPU 2000000UL


volatile uint8_t counter=0;
volatile uint8_t display_flag;
int dispArray[10];

int main(void)
{
	/* Replace with your application code */
	gpio_init();
	timer_init();
	init_array_portc(dispArray);
	
	sei();
	
	
	while (1)
	{
		if (counter<99) {
			counter++;
			_delay_ms(1000);
		}
		else {
			counter = 0;
		}
	}
}

