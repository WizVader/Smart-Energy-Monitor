/*
 * Display.h
 *
 * Created: 6/10/2023 3:56:58 pm
 *Author: Charanjeet, Sai Kiran, Kristelle, Pratham
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

void init_display();

void displayCurrent();

void displayVoltage();

void displayPower();

void seperate_and_load_characters(uint32_t number, uint8_t decimal_pos);

void send_next_character_to_display();

#endif /* DISPLAY_H_ */