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

float adc_process_voltage(uint16_t value);

float adc_process_current(uint16_t value);

float calculate_rms(float voltage_vals[], uint8_t arr_size,uint8_t type);

void adc_conversion_trigger();

float calculate_power(float Vrms, float Irms);

#endif /* ADC_H_ */