/*
 * Display.c
 *
 * Created: 6/10/2023 3:55:19 pm
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Display.h"
#include "Timer0.h"
#define F_CPU 2000000UL
const uint8_t segPattern[10] = {0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};

//4 characters to be displayed on Ds1 to Ds 4
static volatile uint8_t disp_characters[4]={0,0,0,0};

//The current digit (e.g. the 1's, the 10's) of the 4-digit number we're displaying
static volatile uint8_t disp_position=0;



void init_display() {
	
	// Configure SH_ST, SH_DS & SH_CP as outputs
	DDRC |= (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
		
	// Set DS1...DS4 as outputs
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
		
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 <<PORTD6) | (1 << PORTD7);
}

void seperate_and_load_characters(uint32_t number, uint8_t decimal_pos){
	
	//1. Separate each digit from ‘number’
	// e.g. if value to display is 1230 the separated digits will be
	// ‘1’, ‘2’, ‘3’ and ‘0’
	//2. Lookup pattern required to display each digit from ‘seg_pattern[]’
	// and store this pattern in appropriate position of ‘disp_characters[]’
	// e.g. For digit ‘0’ in example above disp_characters[0] = seg_pattern[0]
	//3. For the project you may modify this pattern to add decimal point at
	// the position ‘decemal_pos’
	
	
	// Seperate each digit from the number
	for (int i = 0; i < 4; i++) {
		uint8_t seperateDigit = number % 10;
		disp_characters[i] = segPattern[seperateDigit];

		if (i == decimal_pos) {
			disp_characters[i] |= (1 << 7);
		}
		number /= 10;
	}
}

//Render a single digit from ‘disp_characters[]’ on the display at ‘disp_position’
void send_next_character_to_display(){
	
	// Based on ‘disp_position’, load the digit to send to a local variable
	uint8_t digit = disp_characters[disp_position];
		
	//2. Send this bit pattern to the shift-register as in Q2.2
	for (int i = 7; i >= 0; i--) {
		
		if (digit & (1 << i)) {
			PORTC |= (1 << PORTC4);
		} 
			
		else {
			PORTC &= ~(1 << PORTC4);
		}
		
		
		PORTC |= (1 << PORTC3);
		PORTC &= ~(1 << PORTC3);
	}
	
	
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 <<PORTD6) | (1 << PORTD7);
	PORTC |= (1 << PORTC5);
	PORTC &= ~(1 << PORTC5);
	
	if (disp_position==0) {
		PORTD &= ~(1 << PORTD7);
		disp_position++;
	}

	else if(disp_position==1) {
		PORTD &= ~(1 << PORTD6);
		disp_position++;
	}
	else if(disp_position==2) {
		PORTD &= ~(1 << PORTD5);
		disp_position++;
	}
	else {
		PORTD &= ~(1 << PORTD4);
		disp_position=0;
	}
}

