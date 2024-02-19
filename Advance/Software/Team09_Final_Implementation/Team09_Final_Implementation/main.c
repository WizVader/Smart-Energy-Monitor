
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
// Header includes
#include "uart.h"
#include "Display.h"
#include "ADC.h"
#include "Timer0.h"

//Creating a stream variable
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

// Initialise variables and arrays needed for ADC0 and ADC1
float ADC0[60];
float ADC1[60];
uint16_t rawADC0[60];
uint16_t rawADC1[60];

// Initialise RMS variables, Peak Current, and Power
float VrmsValue = 0;
float IrmsValue = 0;
float PeakCurrent = 0;
float power = 0;
uint8_t PowerInteger = 0;

// Initialise variables that are used for printing purposes
uint8_t VrmsInteger = 0;
uint8_t VrmsDecimal = 0;
uint16_t IrmsDecimal = 0;
uint16_t PeakCurrentDecimal = 0;
uint16_t PowerDecimal = 0;

// Initialise check and counter values
volatile uint8_t counter = 0;
volatile uint8_t checker = 0;

//Initialise amount of samples we want to take
uint8_t samples = 60;

// Set calculated inductive reactance value
float inductive_reactance = 12.56637;

// Initialise variables used for display
uint8_t decimalPos = -1;
int four_digit = 0;


int main(void)
{
	//Binding the stream variable to stdout
	stdout = &usart_stdout;
	
	// Set ports used for debugging
	DDRB |= (1<<DDB5);
	DDRD &= ~(1<<DDD2);
	
	//initialising display and timer
	init_display();
	timer0_init();
	
	// Initialise UART 
	usart_init(12); // UBRR set to 12 for 9600 baud rate
	
	// Initialise INT0 to trigger during rising edge
	EICRA |= (1<<ISC01) | (1<<ISC00);
	
	// Enable global interrupts
	sei();
	
	while (1) {
		
		// Poll until all samples are taken for voltage and current sensing
		while (checker == 0) {
			// Start ADC conversions
			adc_conversion_trigger();
		}
		
		
		// Start processing ADC values after all samples are taken
		if (checker == 1) {
			
			// Process each sample
			for(int i = 0; i<samples; i++){
				// Converted to mV
				ADC0[i] = adc_process_voltage(rawADC0[i]);
				// Converted to mA
				ADC1[i] = adc_process_current(rawADC1[i]);
			}
			
			
			// Apply Riemann Sum to acquire RMS value
			VrmsValue = calculate_rms(ADC0, samples,0); // in V 
			IrmsValue = calculate_rms(ADC1, samples,1); // in mA
			
			// Convert IrmsValue to PeakCurrent
			PeakCurrent = IrmsValue * sqrt(2);
			
			// Calculate power in mW using Vrms, Irms, and power factor
			power = calculate_power(VrmsValue, IrmsValue);
			
			// Separate float values into integers for 4 digit printing to match display
			VrmsInteger = (int) VrmsValue; // Get integers
			VrmsDecimal = (int)(VrmsValue*100) % 100; // Get 2 dp
			PeakCurrentDecimal = (int)(PeakCurrent*1000); // Get 4 dp
			PowerInteger = (int)(power); // Get integers
			PowerDecimal = (int)(power*1000) % 1000; // Get 3 dp
			
			// Display "VOLT"
			displayVoltage();
			
			_delay_ms(1000); // Delay 1s for readability
			
			// Sends the correctly formatted number to the display
			int toSend = float_to_integer(VrmsValue);
			seperate_and_load_characters(toSend, decimalPos);
			
			// Prints the RMS Voltage
			printf("RMS Voltage is: %d.%dV\r\n", VrmsInteger,VrmsDecimal);
			
			_delay_ms(1000); // Delay 1s for readability
			
			// Displays "CRNT"
			displayCurrent(); 
			
			_delay_ms(1000); // Delay 1s for readability
			
			// Sends the correctly formatted number to the display
			toSend = float_to_integer(PeakCurrent);
			seperate_and_load_characters(toSend,decimalPos);
			
			// Prints Peak Current
			printf("Peak Current is: %dmA\r\n", PeakCurrentDecimal);
			
			_delay_ms(1000); // Delay 1s for readability
			
			
			// Displays "POW"
			displayPower();
			
			_delay_ms(1000); // Delay 1s for readability
			
			// Sends the correctly formatted number to the display
			toSend = float_to_integer(power);
			seperate_and_load_characters(toSend,decimalPos);
			
			// Prints the Power 
			printf("Power is: %d.%dW\r\r\n", PowerInteger, PowerDecimal);
			
			_delay_ms(1000); // Delay 1s for readability
			
			// Set checker to 0 again to start taking samples
			checker = 0;
		}
	}
	
}
