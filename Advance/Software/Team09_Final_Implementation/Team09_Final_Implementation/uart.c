/*
 * uart.c
 *
 * Created: 14-08-2023 15:39:22
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 

// Adding necessary include statements
#include <stdio.h>
#include <avr/io.h>
// Header includes
#include "uart.h"

// Defining the character size to 8 bits
#define SET_CHARACTER_BIT UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01)

// Enabling the Transmit functionality of the AVR Microcontroller
#define TRANSMIT_ENABLE UCSR0B |= (1<<TXEN0)

// Creating our own printf method to print a stream of characters
int uart_printf(char var, FILE *stream) {
    usart_transmit(var);        //Using our original function to transmit UART data
    return 0;
}

// Function to transmit byte
void usart_transmit(uint8_t data) {
	
	// UDRE0 bit is checked to see if the bit is low (0)
    while (~UCSR0A &(1<<UDRE0)) {
        ;						// if UDRE0 is not low, wait until it becomes low (0)
    }
	// Load UDR0 bit with the byte to be transmitted noce UDRE0 becomes 0.
    UDR0 = data;
}

// This function configures the UART to 9600 baud, 8N1 with no parity
void usart_init(uint16_t ubrr) {
	
	// Calling the defined variables within this function to help setup the UART
    SET_CHARACTER_BIT;
    TRANSMIT_ENABLE;
	
	// Setting the UBRR0 as per baud rate formula 
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
}