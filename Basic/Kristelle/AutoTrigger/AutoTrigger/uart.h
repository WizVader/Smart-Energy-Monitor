/*
 * uart.h
 *
 * Created: 14-08-2023 15:40:03
 *  Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#ifndef UART_H_
#define UART_H_

// only required include statement placed here
#include <stdint.h>

// Function Prototypes
int uart_printf(char var, FILE *stream);
void usart_transmit(uint8_t data);
void usart_init(uint16_t ubrr);

#endif /* UART_H_ */