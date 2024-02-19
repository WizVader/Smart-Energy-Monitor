/*
 * ADC.c
 *
 * Created: 6/10/2023 4:46:15 pm
 *  Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#define F_CPU 2000000

// Adding necessary include statements
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "Display.h"
#include "ADC.h"

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
	
	while((ADCSRA & (1<< ADIF)) == 0){ // ADIF bit is checked to see if it is 0
		;                              // If ADIF bit is not 1, wait till it becomes 1
	}
	
	return ((ADCL << 0) | (ADCH << 8)); // Alternatively you can write return ADC
}

// Process ADC data from voltage sensing to milliVolts
uint16_t adc_process_voltage(uint16_t value){
	
	// Vac = (ADC * 5 / 1024 - Voff) / (Gvs*Gvo)
	uint16_t Vac_mv =(((uint32_t)value*(Vref/resolution)) - 2.1)/(2.7/58.7);
	return Vac_mv;
}

// Process ADC data from current sensing to milliAmps
uint16_t adc_process_current(uint16_t value){
	
	// Iload = (ADC * 5 / 1024 - Voff) / (Gis*Gio)
	uint16_t Iload_mA = (((uint32_t)value*Vref/resolution) - 2100)/(50/47);
	return Iload_mA;	
}


// Calculate the Power
uint32_t powerCalculation(uint16_t ADC0[], uint16_t ADC1[], uint16_t voltageApprox[], uint16_t currentApprox[]){
	
	// initialize arrays and variables
	uint32_t powerArray[40];
	uint32_t power = 0;
		
	// Only loop 38 times as we did not approximate the 40th voltage or 1st current values
	for(int i = 1; i<39; i++){
		// Power can be evalued by (measured voltage * approximated current) + (approximated voltage * measured current)
		powerArray[i] = ((uint32_t)ADC0[i] * (uint32_t)currentApprox[i]) + ((uint32_t)ADC1[i] * (uint32_t)voltageApprox[i]); 
		// Add all the power values together
		power += powerArray[i];
		
	}
	
	// Divide power by the amount of samples, including linear approximated values, then divide by 1000 to get value in mW
	power /= 2*39000;
	
	
	return power;
} 
