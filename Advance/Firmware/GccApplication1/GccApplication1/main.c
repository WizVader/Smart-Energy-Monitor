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
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "Display.h"
#include "ADC.h"
#include <math.h>



//Creating a stream variable
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

// Defining values for RMS Voltage, Peak Current and Power Consumption to be transmitted.
//#define RMSVoltage 14.5
uint32_t RMSVoltage = 0;
//#define RMSVoltage 12.5
#define PeakCurrent 12.5
//uint32_t power = 0;
uint8_t decimalPos = -1;
int four_digit = 0;



// Initialise arrays for ADC0 and ADC1
uint16_t ADC0[40];
uint16_t ADC1[40];
// Initialise arrays for linear approximation values 
uint16_t voltageLinearApprox[40];
uint16_t currentLinearAprrox[40];
uint32_t powerArray[40];


uint32_t RMSVoltageCalc(uint16_t ADC0[40], int nSamples) {
	
	// Calculate the squared voltage
	// sum all voltage values
	// Calculate the mean
	// Square the mean voltage value
	
	uint32_t sumVoltage = 0;
	for (int i = 0; i < nSamples; i++) {
		
		// Square the voltage values
		//sumVoltage += (uint32_t)ADC0[i] * (uint32_t)ADC0[i];	
		ADC0[i]/1000;	
		sumVoltage += pow(ADC0[i], 2);
		sumVoltage = abs(sumVoltage);
	}
	
	//double step = 1 / (double)nSamples;
			
	// Calculate the mean
	uint32_t meanVoltage = sumVoltage / nSamples;
		
	// Square the mean voltage and return it
	return sqrt(meanVoltage);
}


int isInteger(float value) {
	return value == (int)value;
}

uint32_t sendWholeNumber(float num, int dec) {
	return (int)num*(pow(10,(dec+1)));
	
}


int findDecimalPoint(float num) {
	int position = 0;
	float tempNum = num;
	
	while(tempNum - (int)tempNum != 0) {
		tempNum *= 10;
		position++;
	}
	return position;
}


int float_to_integer(float toDisplay){
	if((int)(toDisplay) == 0){
		decimalPos = -1;
		four_digit = (int)(toDisplay*10000);
	}	
	else if (toDisplay < 10 && toDisplay > 0){
		decimalPos = 3;
		four_digit = (int)(toDisplay*1000);
	}
	else if (toDisplay <100 && toDisplay>= 10){
		decimalPos = 2;
		four_digit = (int)(toDisplay*100);
	}
	else if (toDisplay < 1000 && toDisplay >= 100){
		decimalPos = 1;
		four_digit = (int) (toDisplay*10);
	}
	else{
		four_digit = (int)(toDisplay);
		decimalPos = -1;
	}
	
	return four_digit;
}
	


int main(void)
{
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
 		// Take 80 samples, alternating between ADC0 and ADC1
 		for(int i = 0; i<40; i++){
 
 			// Read from ADC0
 			uint16_t readADC0 = adc_read(0);
 			// Convert to mv
 			uint16_t ADC0_mv = adc_process_voltage(readADC0);
 			// Add to array
 			ADC0[i] = ADC0_mv;
			  			
 			// Read from ADC1
 			uint16_t readADC1 = adc_read(1);
 			// Convert to mA
			uint16_t ADC1_mA = adc_process_current(readADC1);
			// Add to array
			//ADC1[i] = ADC1_mA;
			ADC1[i] = readADC1;
 		}

		for (int i = 0; i < 40; i++) {
			
			printf("%d", ADC1[i]);
			printf("\r\n");
		}
		
		RMSVoltage = RMSVoltageCalc(ADC0[40], 40);
		
		
		// Use the processed voltage and current values and take linear approximation between two points
		for(int k = 0; k<40; k++){
			
			// For voltage approximation, last value is not approximated
			if(k<39){
				voltageLinearApprox[k] = (ADC0[k] + ADC0[k+1])/2;
			}
			
			// For current approximation, first value is not approximated
			if(k>0){
				currentLinearAprrox[k] = (ADC1[k-1] + ADC1[k])/2;
			}
		}
	
// 		// Power Calculation
// 		power = powerCalculation(ADC0, ADC1, voltageLinearApprox, currentLinearAprrox);
// 		printf("Power is: %lu \r\n", power);
		
		//seperate_and_load_characters(power, 3);
		
        // Printing the RMS Voltage while separating and printing each number of the double value
        printf("RMS Voltage is: %d%d.%d\r\n", (int)RMSVoltage/10 , (int)RMSVoltage%10, (int)(RMSVoltage*10)%10 );
        		
		//float current = 114.5;
		
		//uint32_t intValue = (uint32_t)(current * 100);	
		
		//int decimalPos = findDecimalPoint(current);
		//printf("NUMBER: %d\r\n", sendWholeNumber(current));
		
		//printf("float_to_int is %d, dp id %d\r\n", toSend, decimalPos);
		
		// Temporary Global variables
		float voltage = 14.30;
		float current = 130.15;
		float power = 2.542;
				
				
		displayCurrent();
		
		 _delay_ms(1000);
		 
		 
		int toSend = float_to_integer(current);
		seperate_and_load_characters(toSend, decimalPos);	
		
		printf("Decimal position %d expected 1\r\n", decimalPos);
		
		_delay_ms(1000);	
		
		displayVoltage();
		
		
		_delay_ms(1000);
		
		 toSend = float_to_integer(voltage);
		seperate_and_load_characters(toSend, decimalPos);	
		printf("Decimal position %d expected 2\r\n", decimalPos); 
		
		_delay_ms(1000);
		
		displayPower();
		
		_delay_ms(1000);
		
		 toSend = float_to_integer(power);
		seperate_and_load_characters(toSend, decimalPos);
		printf("Decimal position %d expected 3\r\n", decimalPos);
		
        // delay of 1s
        _delay_ms(1000);
        
        //printing the Peak Current
        printf("Peak Current is: %d\r\n", PeakCurrent);
        // delay of 1s
        _delay_ms(1000);
        
        // Printing the Power while separating and printing each number of the double value
        //printf("Power is: %d.%d%d\r\n", (int)Power%10, (int)(Power*10)%10, (int)(Power*100)%10);
        
        // delay of 1s
        _delay_ms(1000);
        
        //printing a blank line before the next iteration
        printf("\r\n");
    }
 
}