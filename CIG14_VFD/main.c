/*
 * CIG14_VFD.c
 *
 * Created: 07.05.2023 18:52:06
 * Author : User
 */ 

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "string.h"
#include "stdbool.h"
#include "lib/uart_hal.h"
#include "lib/gpio_driver.h"
#include "lib/twi_hal.h"
#include "lib/adc_hal.h"
#include "lib/rtc.h"
#include "lib/ISL29034.h"
#include "lib/cig14.h"


uint8_t light_addr = 0x44;

gpio ld1 = {(uint8_t *)&PORTB , PORTB5};
gpio rtc_int = {(uint8_t *)&PORTD , PORTD2};
gpio p_good = {(uint8_t *)&PORTC , PORTC5};
gpio lcd_blk = {(uint8_t *)&PORTE , PORTE2};




rtc_date sys_rtc = {0, 42, 13, 8, 5, 23, 2};
//second, minute, hour, date, month, year, dayofweek


uint8_t rtc_int_request = 0;

ISR(INT0_vect){
	rtc_int_request=1; //RTC New data ready
}


int main(void)
{
	char char_array[128]="\0";
	uart_init(250000,0);
	twi_init(400000);
		
	EICRA |= (0b10 << ISC00);
	EIMSK = 0x01; //0b00000001
	
	set_pin_dir(&ld1 , PORT_DIR_OUT); set_pin_level(&ld1, false);
	set_pin_dir(&rtc_int, PORT_DIR_IN);
		
	sei();
	
	//rtc_set(&sys_rtc);
	rtc_int_enable(&sys_rtc ,0);
	adc_init();
	ISL29034_init(POWER_RUN, RES_16_BIT, LUX_4000);
	vfd_init(6);
	vfd_set_brightness(0xff);
	
	uart_send_string((uint8_t *)"Program Start\n\r");
	
	
	//sleep_enable();
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
    /* Replace with your application code */
    while (1) 
    {
		if (rtc_int_request != 0){
			rtc_int_request = 0;
			//set_pin_level(&ld1, true);
			rtc_sync(&sys_rtc);
			sprintf(char_array, "%02d:%02d", sys_rtc.hour, sys_rtc.minute);
			//_delay_ms(1);
			//sleep_cpu();
		}
		
		//sprintf(char_array, "%05u", read_light_intensity());
		//vfd_set_brightness(read_light_intensity());
		vfd_string(0, (uint8_t *)char_array);
		uart_send_string((uint8_t *)char_array);
		uart_send_string((uint8_t *)"\n\r");
		_delay_ms(100);
		
    }
}

