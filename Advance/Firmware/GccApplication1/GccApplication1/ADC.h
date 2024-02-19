/*
 * ADC.h
 *
 * Created: 6/10/2023 4:46:29 pm
 *  Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h> //Necessary for definitions of uint8_t etc

// Add functions used in ADC.c
void adc_init();
uint16_t adc_read(uint8_t chan);
uint16_t adc_convert_mv(uint16_t value);
uint16_t adc_process_voltage(uint16_t value);
uint16_t adc_process_current(uint16_t value);
uint32_t powerCalculation(uint16_t ADC0[], uint16_t ADC1[], uint16_t voltageApprox[], uint16_t currentApprox[]);

#endif /* ADC_H_ */