/*
 * Timer0.c
 *
 * Created: 12/10/2023
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#include <avr/io.h>
#define F_CPU 2000000UL
#include <stdio.h>
#include "Timer0.h"

void timer0_init() {

	TCCR0A |= (1 << WGM01);

	TCCR0B |= (1 << CS02);

	OCR0A = 77;

	TIMSK0 |= (1 << OCIE0A);

}





