/** timer0.c
*
* Created: 23/09/2023 11:31:24 am
*  Author: saika
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "timer0.h"
#include "led.h"

volatile uint16_t millisecond_counter;
volatile uint16_t overflowcounter;
volatile uint16_t pulse_width_us;
volatile uint16_t starting;
volatile uint16_t ending;
#define F_CPU 2000000UL


void timer0_init() {
	//TODO: initialize and configure timer0 to count to 10ms
	//TCCR0A |= (1<<WGM01);
	
	//Timer operating at normal mode
	//TCCR0B |= (1<<CS00);
	TIMSK0 |= (1 << TOIE0);  // Enable Timer0 Overflow Interrupt
	
		
	// setting the comparable top value to 77 which is an indirect calculation that means
	// the period of event occurring is 10ms (Top = (period/resolution) - 1)
	
	//OCR0A |= (1<<OCR0A_6) | (1<<OCR0A_3) | (1<<OCR0A_2) | (1<<OCR0A_0); //OCR0A |= 77
	
	// enabling the interrupt to be able to use it first
	//TIMSK0 |= (1<<OCIE0A);
}

uint8_t timer0_check_clear_compare() {
	//TODO: check compare flag
	if( TIFR0 & (1 << OCF0A)) {
		//TODO: clear compare flag.
		//Note: in data sheet this is done by writing 1 to the compare flag
		TIFR0 |= (1 << OCF0A);
		return 1;
	}
	return 0;
}



ISR(TIMER0_COMPA_vect) {
	// This ISR function is called when timer0 reaches compare value.
	// Compare flag is automatically cleared.
	//led_toggle();
	millisecond_counter++; // incrementing the millisecond counter every time the compare flag is set
}

ISR(INT0_vect){
	
	if(EICRA & (1<<ISC00)){ // if configuration was for rising edge detection
		
		TCCR0B |= (1<<CS00);// starting the counter once rising edge
		TCNT0 = 0x00; //clear the counter
		//starting = TCNT0;
		overflowcounter = 0;
		//EICRA |= (1<<ISC01);
		EICRA &= ~(1<<ISC00); // Falling edge
		
		} else { //else configuration was set for falling edge detection
		
		//currentcount = TCNT0;// read the timer/counter0 value
		ending = TCNT0;
		pulse_width_us = ((uint64_t)256 * overflowcounter + (ending+1)) * 1000000UL / F_CPU; //calculating in microseconds
		TCCR0B &= ~(1<<CS00) | ~(1<<CS01) | ~(1<<CS02); //stopping the counter
		EICRA |= (1<<ISC00); // configure to rising edge
		
	}
}

ISR(TIMER0_OVF_vect){
	overflowcounter++; // if overflow occurs increment
}