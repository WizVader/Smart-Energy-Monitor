/*
 * Display.c
 *
 * Created: 6/10/2023 3:55:19 pm
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


// Adding necessary include statements
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
// Header includes
#include "Display.h"
#include "Timer0.h"

// Hex code to display 0-9 in the 7 segment display
const uint8_t segPattern[10] = {0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};

// Custom words 
const uint8_t crntPattern[] = {0x78, 0x37, 0x31, 0x39}; // "CRNT"
		
const uint8_t voltPattern[] = {0x78, 0x38, 0x3F, 0x3E}; // "VOLT"
	
const uint8_t powPattern[] = {0x1E, 0x3C, 0x3F, 0x73}; // "POW"

// 4 characters to be displayed on Ds1 to Ds 4
static volatile uint8_t disp_characters[4]={0,0,0,0};

//The current digit (e.g. the 1's, the 10's) of the 4-digit number we're displaying
static volatile uint8_t disp_position=0;

// External variables from main.c
extern uint8_t decimalPos; // Where to play decimal point
extern int four_digit; // Integer to display without the decimal point

// Display the next character every ~10ms
ISR(TIMER0_COMPA_vect) {
	send_next_character_to_display();
}


// Initialise the display
void init_display() {
	
	// Configure SH_ST, SH_DS & SH_CP as outputs
	DDRC |= (1 << DDC3) | (1 << DDC4) | (1 << DDC5);
		
	// Set DS1...DS4 as outputs
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	
	// Set to logic high
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 <<PORTD6) | (1 << PORTD7);
}


// Function to display "CRNT"
void displayCurrent() {
	
	// Loop through digits
	for (int i = 0; i < 4; i++) {
		// Add to the array of characters to displays
		disp_characters[i] = crntPattern[i];
	}
}

// Function to display "VOLT"
void displayVoltage() {
	
	// Loop through digits
	for (int i = 0; i < 4; i++) {
		// Add to the array of characters to displays
		disp_characters[i] = voltPattern[i];
	}
}

// Function to display "POW"
void displayPower() {
	
	// Loop through digits
	for (int i = 0; i < 4; i++) {
		// Add to the array of characters to displays
		disp_characters[i] = powPattern[i];
	}
	
}


// Function to get a number and separate each digit and lload
void seperate_and_load_characters(uint32_t number, uint8_t decimal_pos){
	
	// Separate each digit from the number
	for (int i = 0; i < 4; i++) {
		
		// Starts from the least significant number
		uint8_t seperateDigit = number % 10;
		
		// Check segPattern to see which pattern should be display
		disp_characters[i] = segPattern[seperateDigit];
		
		// Turn on decimal place if needed
		if (i == decimal_pos) {
			disp_characters[i] |= (1 << 7);
		}
		
		// Divide by 10 to get the next least significant number
		number /= 10;
	}
}


//Render a single digit from ‘disp_characters[]’ on the display at ‘disp_position’
void send_next_character_to_display(){
	
	// Based on ‘disp_position’, load the digit to send to a local variable
	uint8_t digit = disp_characters[disp_position];
		
	//2. Send this bit pattern to the shift-register as in Q2.2
	// For each bit, start with MSB
	for (int i = 7; i >= 0; i--) {
		
		// Set the SH_DS to be 0 or 1
		if (digit & (1 << i)) {
			PORTC |= (1 << PORTC4);
		} 
			
		else {
			PORTC &= ~(1 << PORTC4);
		}
		
		// Toggle the SH_CP (on then off)
		PORTC |= (1 << PORTC3);
		PORTC &= ~(1 << PORTC3);
	}
	
	// Disable all digits
	PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 <<PORTD6) | (1 << PORTD7);
	
	// Latch the output by toggling SH_ST pin
	PORTC |= (1 << PORTC5);
	PORTC &= ~(1 << PORTC5);
	
	// Depending on the value value of the position, enable the correct digit
	if (disp_position==0) {
		PORTD &= ~(1 << PORTD7); // Enable Ds4
		disp_position++; 
	}

	else if(disp_position==1) {
		PORTD &= ~(1 << PORTD6); // Enable Ds3
		disp_position++;
	}
	else if(disp_position==2) {
		PORTD &= ~(1 << PORTD5); // Enable Ds2
		disp_position++;
	}
	else {
		PORTD &= ~(1 << PORTD4); // Enable Ds1
		disp_position=0;
	}
}


// Function to convert floats to integers
int float_to_integer(float toDisplay){
	
	// Float is 0.XXXX
	if((int)(toDisplay) == 0){
		decimalPos = 1; // Decimal point displayed at the end of the number
		four_digit = (int)(toDisplay*10000); // Formats to a 4 digit integer
	}
	
	// Float is X.XXX
	else if (toDisplay < 10 && toDisplay > 0){
		decimalPos = 3; // Has 3 dp
		four_digit = (int)(toDisplay*1000); // Formats to a 4 digit integer
	}
	
	// Float is XX.XX
	else if (toDisplay <100 && toDisplay>= 10){
		decimalPos = 2; // Has 2 dp
		four_digit = (int)(toDisplay*100); // Formats to a 4 digit integer
	}
	
	// Float is XXX.X
	else if (toDisplay < 1000 && toDisplay >= 100){
		decimalPos = 1; // Has 1 dp
		four_digit = (int) (toDisplay*10); // Formats to a 4 digit integer
	}
	
	// Float is is either XXXX or has more than 5 digits 
	else{
		four_digit = (int)(toDisplay); // Formats to a 4 digit integer
		decimalPos = -1; // No decimal point displayed
	}
	
	return four_digit;
}

