/*
 * Timer0.c
 *
 * Created: 12/10/2023
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


// Adding necessary include statements
#include <avr/io.h>
#define F_CPU 2000000UL
#include <stdio.h>
// Header includes
#include "Timer0.h"


// Function to initialise Timer0/Counter to count every ~10ms
void timer0_init() {

	// Set to CTC Mode
	TCCR0A |= (1 << WGM01);

	// Set 256 Prescaler
	TCCR0B |= (1 << CS02);
	
	// Set TOP value as 77
	OCR0A = 77;
	
	// Enable Compare Match A interrupt
	TIMSK0 |= (1 << OCIE0A);

}





