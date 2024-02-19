/*
 * led.c
 *
 * Created: 23/09/2023 11:33:16 am
 *  Author: saika
 */ 
 
#include <avr/io.h>
#include "led.h"

void led_toggle(){
	 //TODO: LED toggle code
	 //This line toggles the state of PB5. 
	 //If PB5 was previously HIGH, it becomes LOW. 
	 //If it was LOW, it becomes HIGH.
	 PORTB ^= (1<<PORTB5);
}