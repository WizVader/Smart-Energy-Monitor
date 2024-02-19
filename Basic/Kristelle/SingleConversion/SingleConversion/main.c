/*
 * Team_09_Firmware.c
 *
 * Created: 14-08-2023 12:27:34
 * Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 

#define F_CPU 2000000UL

// Adding necessary include statements
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "uart.h"
#include "Display.h"
#include "ADC.h"
#include "Timer0.h"

//Creating a stream variable
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

// Initialise variables and arrays for ADC0 and ADC1
float ADC0[12];
float ADC1[12];
uint16_t rawADC0[12];
uint16_t rawADC1[12];
float VrmsArray[12];
float IrmsArray[12];
float VrmsValue = 0;
float IrmsValue = 0;
uint8_t VrmsInteger = 0;
uint8_t VrmsDecimal = 0;
uint8_t IrmsInteger = 0;
uint16_t IrmsDecimal = 0;
volatile uint8_t counter = 0;
volatile uint8_t checker = 0;


int main(void)
{
	DDRB |= (1<<DDB5);
	
    //Binding the stream variable to stdout
    stdout = &usart_stdout;
    
    // Using the calculated ubrr value of 12 to initialise uart 
    usart_init(12);
	
	init_display();
	timer0_init();
	
	// Initialise the ADC
	adc_init();

	sei();
		
    while (1) {
		
		printf("In the while\r\n");
		
			//Read and process each sample
			for(int i = 0; i<24; i++){
				
				// Read voltage
				if(counter < 12){
					// add delay after 6th sample is read
					if (counter==6) {
	
						_delay_us(260);
					}
					// read samples
					rawADC0[counter] = adc_read(0);
					counter++;
					//printf("Reading voltage");
				}
				
				// Read current
				else {
					// delay afet 18th sample
					if(counter == 18){
						_delay_us(260);
					}
					rawADC1[counter-12] = adc_read(1);
					counter++;
					//printf("Reading current");
				}
				// Reset values
				
				if(counter >= 24){
					counter = 0;
					checker = 1;
				}
				
				
				
			}
			
			printf("done reading \r\n");
			
			// process 
			for(int i = 0; i<12; i++){
				// Convert to mV
				ADC0[i] = adc_process_voltage(rawADC0[i]);
				// Convert to mA
				ADC1[i] = adc_process_current(rawADC1[i]);
			}
			printf("done processing \r\n");
			
			
			VrmsValue = calculate_rms(ADC0, 12);
			IrmsValue = calculate_rms(ADC1, 12);
			
			
			VrmsInteger = (int) VrmsValue;
			VrmsDecimal = (int)(VrmsValue*100) % 100;
			IrmsDecimal = (int)(IrmsValue*1000)%1000;
			printf("Vrms is %d.%dV\r\n", VrmsInteger,VrmsDecimal);
			printf("Irms is %d mA\r\n", (int)(IrmsValue*1000));
			
			
			checker = 0;
			
		
    }
 
}