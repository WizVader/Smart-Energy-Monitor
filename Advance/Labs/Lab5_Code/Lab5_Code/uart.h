/*
 * uart.h
 *
 * Created: 6/09/2023 6:54:16 pm
 *  Author: SAIKIRAN N KAMAT
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

// Function Prototypes
int uart_printf(char var, FILE *stream);
void usart_transmit(uint8_t data);
void usart_init(uint16_t ubrr);


#endif /* UART_H_ */