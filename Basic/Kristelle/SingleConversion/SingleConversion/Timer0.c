/*
 * Timer0.c
 *
 * Created: 12/10/2023
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#define F_CPU 2000000UL

// Adding necessary include statements
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "Display.h"
#include "ADC.h"

// Call ISR when Timer0/Count compare matches A
ISR(TIMER0_COMPA_vect) {
	send_next_character_to_display();

}

// Initialise Timer0/Count
void timer0_init() {
	// Enable CTC mode
	TCCR0A |= (1 << WGM01);
	
	// No Prescaler 
	TCCR0B |= (1 << CS00);

	// Set TOP value as 199 for 100us 
	OCR0A = 220;

	// Enable Interrupt 
	TIMSK0 |= (1 << OCIE0A);

}





