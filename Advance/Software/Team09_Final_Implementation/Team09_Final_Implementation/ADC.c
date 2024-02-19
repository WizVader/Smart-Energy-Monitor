
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
// Header includes
#include "uart.h"
#include "Display.h"
#include "ADC.h"
#include "Timer0.h"

// Initialise reference voltage and the resolution
uint16_t Vref = 5000;
uint16_t resolution = 1024;

// Take the variables and arrays from main.c needed to operate the ADC
extern uint16_t rawADC0[60];
extern uint16_t rawADC1[60];
extern volatile uint8_t counter;
extern volatile uint8_t checker;
extern float inductive_reactance;
extern uint8_t samples;

// Initialise variables used for indexing and checking conditions
uint8_t indexing = 0;
uint8_t switch_case = 0;

// This ISR is called when an ADC conversion is completed
ISR(ADC_vect){
	
	// Ensures that we are taking the correct amount of samples for the correct type of sensing circuit
	if (counter<samples && switch_case < 2) {
		
		// Case where ADC voltage sensing values are taken
		if(switch_case == 0){
			// Read from ADC0
			ADMUX &= ~(1<<MUX0);
			// Add ADC reading to  array
			rawADC0[counter] = ADC;
			// Increment to the next sample
			counter++;
		}
		
		// Case where ADC current sensing values are taken
		else if (switch_case == 1){
			// Read from ADC1
			ADMUX |= (1<<MUX0);
			// Add ADC reading to array
			rawADC1[counter] = ADC;
			// Increment to the next sample
			counter++;
		}
	}
	
	// Finished reading ADC samples
	else {
		// Turn off ADC
		ADCSRA &= ~(1 << ADSC);
		ADCSRA &= ~(1 << ADEN);
		// Reset counter
		counter = 0;
		// Go to the next case
		switch_case++;
		
		// After current sensing voltage is done, reset it back to 0
		if(switch_case == 2){
			switch_case = 0;
			// Stops the polling, initailised in the main
			checker = 1;
		}
	}
}


// Function that initialises the ADC conversion 
void adc_conversion_trigger() {
	
	// Start the ADC conversion at rising edge when INT0 flag is set
	if (PIND & (1<<PIND2)) {
		// Manually clear the flag
		EIFR |= (1<<INTF0);
		
		// Start conversion if voltage or current sensing values have not been sampled
		if(switch_case == 0 || switch_case == 1){
			adc_init(); // Conversion started by initialising ADC
		}
		
		// Once current sensing is done, set the switch case back to voltage sensing
		else if(switch_case == 2){
			switch_case = 0;
		}
	}
}


// Function to initialise ADC
void adc_init() {
	
	// Set Vref as Vcc
	ADMUX |= (1 << REFS0); 
	// Enables ADC, turns on auto trigger, enables interrupt, and starts the conversion 
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1<<ADATE) | (1 << ADIE) | (1<<ADSC);
	// Sets ADC to run in free running mode
	ADCSRB &= ~(1<<ADTS0 | 1<<ADTS1 | 1<<ADTS2);

}

// Function to read ADC values
uint16_t adc_read(uint8_t chan) {
	
	// read from ADCH and ADCL
	ADMUX &= 0xF0; // Clear channel selection
	ADMUX |= chan; // Set the channel to convert
		
	
	// Return the value of the ADCL and ADCH registers
	return ((ADCL << 0) | (ADCH << 8));
}

// Function to process ADC data from voltage sensing to milliVolts
float adc_process_voltage(uint16_t value){
	
	// Vac = (ADC * 5 / 1024 - Voff) / (Gvs*Gvo)
	float Vac_mv =((float)(value*((float)Vref/resolution)) - 2100)/(float)(2.7/58.7);
	return Vac_mv;
}

// Function to process ADC data from current sensing to milliAmps
float adc_process_current(uint16_t value){
	
	// Iload = (ADC * 5 / 1024 - Voff) / (Gis*Gio)
	float Iload_mA = (((value*(Vref/(float)resolution)) - 2100)/(50/(float)(47)));
	return Iload_mA;
}

// Function to calculate RMS using Riemann Sum
float calculate_rms(float voltage_vals[], uint8_t arr_size,uint8_t type){
	float sum_Vac = 0;
	
	// Calculates Vrms in V
	if (type==0) {
		for (int i = 2; i < arr_size; i++){
			// Squares the values
			sum_Vac += (pow(voltage_vals[i],2) * 0.0001)/1000; 
		}
		
		// Divides by number of samples
		sum_Vac /= (arr_size * 0.0001); 
	}
	
	// Calculates Irms in mA
	else {
		for (int i = 2; i < arr_size; i++){
			// Squares the values
			sum_Vac += (pow(voltage_vals[i],2) * 0.0001)/1000; 
		}
		
		// Divides by number of samples
		sum_Vac /= (arr_size * 0.0001); 
	}

	// Return the calculated RMS value
	return (float)(sqrt(sum_Vac/1000.0));
}


// Function to calculate the Real Power
float calculate_power(float Vrms, float Irms){ 
	
	// Finding the total impedance first
	float impedance = Vrms/Irms; 
	
	// Finding the pure resistive value
	float resistance = sqrt(pow(impedance,2)-pow(inductive_reactance,2)); 
	
	// Finding the phase = arctan(X/R)
	float phase = atan(inductive_reactance/resistance); 
	
	// Calculating power as power = Vrms times Irms times cos(phase)
	float power = Vrms*Irms*cos(phase); 
	
	return power;
}