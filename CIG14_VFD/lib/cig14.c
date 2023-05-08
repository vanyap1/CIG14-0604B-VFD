
/*
 * cig14.c
 *
 * Created: 07.05.2023 18:57:42
 * Author: Ivan Prints 
 * Email: princ.va@gmail.com
 */ 
#include "cig14.h"
#include "gpio_driver.h"
#include <util/delay.h>


#define nop() asm volatile ("nop")


gpio vfd_rst = {(uint8_t *)&PORTB , PORTB1};
gpio vfd_en = {(uint8_t *)&PORTB , PORTB0};
gpio vfd_do = {(uint8_t *)&PORTD , PORTD6};
gpio vfd_clk = {(uint8_t *)&PORTD , PORTD7};
	
	
void vfd_init(uint8_t char_places){
	set_pin_dir(&vfd_rst , PORT_DIR_OUT); set_pin_level(&vfd_rst, false);
	set_pin_dir(&vfd_en , PORT_DIR_OUT); set_pin_level(&vfd_en, true);
	set_pin_dir(&vfd_do , PORT_DIR_OUT); set_pin_level(&vfd_do, true);
	set_pin_dir(&vfd_clk , PORT_DIR_OUT); set_pin_level(&vfd_clk, true);
	_delay_ms(10);
	set_pin_level(&vfd_rst, true);
	_delay_ms(10);
	set_pin_level(&vfd_en, false);
	vfd_write_byte(INIT_DISPLAY);
	vfd_write_byte(char_places);
	set_pin_level(&vfd_en, true);
}

void vfd_set_brightness(uint8_t brightness){
	if (brightness < 5){brightness=5;}
	set_pin_level(&vfd_en, false);
	vfd_write_byte(BRIGHTNESS_CMD);
	vfd_write_byte(brightness);
	set_pin_level(&vfd_en, true);
	_delay_ms(1);
	set_pin_level(&vfd_en, false);
	vfd_write_byte(FIRSR_CHAR_POSITION);
	vfd_write_byte(0x00);
	vfd_write_byte(0x00);
	vfd_write_byte(0x00);
	vfd_write_byte(0x00);
	vfd_write_byte(0x00);
	vfd_write_byte(0x00);
	set_pin_level(&vfd_en, true);
	nop();
	set_pin_level(&vfd_en, false);
	vfd_write_byte(UPDATE_SCREEN);
	set_pin_level(&vfd_en, true);
	
	
}


void vfd_write_byte(uint8_t input_data){
	unsigned char n;
	for(n=0; n<8; n++)
	{
		set_pin_level(&vfd_clk, false);            
		if((1 << n) & input_data){
			set_pin_level(&vfd_do, true);
		}else{
			set_pin_level(&vfd_do, false);
		}
		set_pin_level(&vfd_clk, true);               
		nop();
	}
}

void vfd_string(uint8_t position, uint8_t *c){
	uint16_t i = 0;
	set_pin_level(&vfd_en, false);
	vfd_write_byte(FIRSR_CHAR_POSITION+position);
	do{
		vfd_write_byte(c[i++]);
		//i++;	
	}while(c[i] != '\0');
	set_pin_level(&vfd_en, true);
	nop();
	set_pin_level(&vfd_en, false);
	vfd_write_byte(UPDATE_SCREEN);
	set_pin_level(&vfd_en, true);
}