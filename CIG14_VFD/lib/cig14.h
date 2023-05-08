
/*
 * cig14.c
 *
 * Created: 07.05.2023 18:57:42
 * Author: Ivan Prints 
 * Email: princ.va@gmail.com
 */ 

#include <avr/io.h>


#define INIT_DISPLAY				0xe0
#define FIRST_CHAR_POSITION			0x20
#define BRIGHTNESS_CMD				0xe4
#define UPDATE_SCREEN				0xe8





void vfd_init(uint8_t char_places);
void vfd_set_brightness(uint8_t brightness);
void vfd_write_byte(uint8_t data);
void vfd_string(uint8_t position, uint8_t *c);
	
	