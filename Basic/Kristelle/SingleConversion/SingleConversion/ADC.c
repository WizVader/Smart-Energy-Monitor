/*
 * ADC.c
 *
 * Created: 15/10/2023 7:09:57 pm
 *  Author: iamkr
 */ 
/*
* ADC.c
*
* Created: 6/10/2023 4:46:15 pm
*  Author: Charanjeet, Sai Kiran, Kristelle, Pratham
*/


#define F_CPU 2000000UL

// Adding necessary include statements
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Display.h"
#include "ADC.h"
#include "Timer0.h"



// initialise reference voltage and resolution
uint16_t Vref = 5000;
uint16_t resolution = 1024;



// Function to initialise ADC
void adc_init() {
	// Set up ADMUX registers
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << MUX1);
	
	// Set up ADCSRA registers
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2);
	
	// ADCSRB is all cleared
	ADCSRB = 0x00;
	
}
// Read ADC value of a specified channel
uint16_t adc_read(uint8_t chan) {
	
	// read from ADCH and ADCL
	ADMUX &= 0xF0; // Clear channel selection
	ADMUX |= chan; // Set the channel to convert
	
	ADCSRA |= (1<< ADSC); // Starting an ADC conversion
	
	PINB |= (1<<PINB5);

	while((ADCSRA & (1<< ADIF)) == 0){ // ADIF bit is checked to see if it is 0
		;                              // If ADIF bit is not 1, wait till it becomes 1
	}
	
	PINB |= (1<<PINB5);

	return ((ADCL << 0) | (ADCH << 8)); // Alternatively you can write return ADC
}

// Process ADC data from voltage sensing to milliVolts
float adc_process_voltage(uint16_t value){
	
	// Vac = (ADC * 5 / 1024 - Voff) / (Gvs*Gvo)
	float Vac_mv =((float)(value*((float)Vref/resolution)) - 2100)/(float)(2.7/58.7);
	return Vac_mv;
}

// Process ADC data from current sensing to milliAmps
float adc_process_current(uint16_t value){
	
	// Iload = (ADC * 5 / 1024 - Voff) / (Gis*Gio)
	float Iload_mA = ((value*((float)Vref/resolution)) - 2100)/((float)50/47);
	return Iload_mA;
}

float calculate_rms(float voltage_vals[], uint8_t arr_size){
	float sum_Vac = 0;
	for (int i = 0; i < arr_size; i++){
		sum_Vac += (pow(voltage_vals[i],2))/1000;
	}
	
	sum_Vac /= (arr_size); //
	
	return (float)(sqrt(sum_Vac/1000.0));
}
