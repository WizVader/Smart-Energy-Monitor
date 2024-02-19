#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "timer0.h"
#include "led.h"
#include "uart.h"

static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

#define F_CPU 2000000UL

volatile uint16_t pulse_width_us;


int main(void) {
	//uint32_t pulse_width_us = 0;  // Renamed for clarity
	//uint32_t frequency = 0;

	
	stdout = &usart_stdout;

	usart_init(12);

	DDRD &= ~((1<<PORTD2) | (1<<PORTD3));  // Combined bitwise operations

	EIMSK |= (1<<INT0); //enabling interrupt for INT0
	EICRA |= (1<<ISC01) | (1<<ISC00); // setting for pos edge

	timer0_init();
	sei();
	
	
	while(1) {
		//pulse_width_us = ((uint64_t)256 * overflowcounter + (ending+1)) * 1000000UL / F_CPU; //calculating in microseconds
		uint16_t time_period_ms = ((pulse_width_us+20) / 1000)*2;
		uint16_t freqency_hz = (uint16_t)(1000/time_period_ms);
		if ((pulse_width_us>0) & (freqency_hz == 500)) {
			printf("The frequency is %u Hz\r\n", freqency_hz );
			printf("%u\r\n",pulse_width_us);
		}
	}
}