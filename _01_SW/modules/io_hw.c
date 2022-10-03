/*
 * io_hw.c
 *
 *  Created on: 20.02.2022
 *      Author: kulet
 */

#include "include/io_hw.h"

#include "stdint.h"
#include "c_types.h"
#include "gpio.h"
#include "eagle_soc.h"


static pin_state_t output_1 = CLEAR;
static pin_state_t output_2 = CLEAR;

/*Public interface implementatnion*/
void io_hw_init(void){

	gpio_init();

	/* GPIO 4 as output*/
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); //@suppress("Suggested parenthesis around expression")
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO4_U);
	gpio_output_set(	0,
						0,
						(1 << RELAY_1_OUT),
						0
	);

	/* GPIO 5 as output*/
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); //@suppress("Suggested parenthesis around expression")
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO4_U);
	gpio_output_set(	0,
						0,
						(1 << RELAY_2_OUT),
						0
	);

	/*GPIO 14 as input*/
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12); //@suppress("Suggested parenthesis around expression")
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTDI_U);
	gpio_output_set(	0,
						0,
						0,
						BIT12
	);

	/*GPIO 15 as input*/
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13); //@suppress("Suggested parenthesis around expression")
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_MTCK_U);
	gpio_output_set(	0,
						0,
						0,
						BIT13
	);

}

pin_state_t io_hw_get_pin(out_pin_t pin){

	return (pin_state_t) GPIO_INPUT_GET(pin);

}


void io_hw_set_pin(out_pin_t pin,pin_state_t state){

	if(INPUT_1 == pin)
	{
		output_1 = state;
	}
	else if(INPUT_2 == pin)
	{
		output_2 = state;
	}
	/*Set corresponding pin*/
	GPIO_OUTPUT_SET(pin, state);

}
