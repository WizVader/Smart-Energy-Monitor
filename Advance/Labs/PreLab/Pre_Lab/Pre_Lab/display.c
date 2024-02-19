/*
* display.c
*
* Created: 27-09-2023 16:44:32
*  Author: svcha
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

#define F_CPU 2000000UL

#define SET_OUTPUT_ANODES_1 DDRC |= 0x3F
#define SET_OUTPUT_ANODES_2 DDRB |= (1<<DDB4)
#define SET_OUTPUT_CATHODES DDRB |= 0x03
#define SET_DS2_LOW PORTB &= ~(1<<PORTB1)
#define SET_DS2_HIGH PORTB |= (1<<PORTB1)
#define SET_DS1_LOW PORTB &= ~(1<<PORTB0)
#define SET_DS1_HIGH PORTB |= (1<<PORTB0)



volatile uint8_t counter;
volatile uint8_t display_flag=0;

void timer_init() {
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02);
	OCR0A = 77;
	TIMSK0 = (1 << OCIE0A);
}

void gpio_init() {
	SET_OUTPUT_ANODES_1;
	SET_OUTPUT_ANODES_2;
	SET_OUTPUT_CATHODES;
	SET_DS2_LOW;
	SET_DS1_HIGH;
}

void init_array_portc(int array[]) {
	array[0] = 0x3F;
	array[1] = 0x06;
	array[2] = 0x1B;
	array[3] = 0x0F;
	array[4] = 0x26;
	array[5] = 0x2D;
	array[6] = 0x3D;
	array[7] = 0x07;
	array[8] = 0x3F;
	array[9] = 0x2F;
}

void disp_portb(int counter) {
	if (counter==0 || counter==1 || counter==7) {
		PORTB &= ~(1<<PORTB4);
	}
	else {
		PORTB |= (1<<PORTB4);
	}
}

ISR(TIMER0_COMPA_vect) {
	if (display_flag==0) {
		SET_DS1_LOW;
		SET_DS2_HIGH;
		PORTC = dispArray[counter/10];
		disp_portb(counter/10);
		display_flag = 1;
	}
	else {
		_delay_ms(100);
		SET_DS1_HIGH;
		SET_DS2_LOW;
		PORTC = dispArray[counter%10];
		disp_portb(counter%10);
		display_flag = 0;
	}
	
}