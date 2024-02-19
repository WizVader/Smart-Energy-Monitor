/*
 * timer0.h
 *
 * Created: 23/09/2023 11:29:23 am
 *  Author: saika
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include <stdbool.h> //Initialize timer0 as per Part 1 

extern volatile uint16_t pulse_width_us;

void timer0_init();
//Using polling check if timer0 has reached comparison value 
//if so, it will clear the compare flag and return 1 
//otherwise, it returns 0
uint8_t timer0_check_clear_compare();

#endif /* TIMER0_H_ */