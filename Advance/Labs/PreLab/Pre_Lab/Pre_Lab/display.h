/*
 * display.h
 *
 * Created: 27-09-2023 16:44:42
 *  Author: svcha
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

void gpio_init();
void init_array_portc(int array[]);
void disp_portb(int counter);
void timer_init();

extern volatile uint8_t counter;
extern volatile uint8_t display_flag;
extern int dispArray[10];

#endif /* DISPLAY_H_ */