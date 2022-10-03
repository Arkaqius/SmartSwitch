/*
 * io_hw.h
 *
 *  Created on: 20.02.2022
 *      Author: kulet
 */

#ifndef CORE_INCLUDE_IO_HW_H_
#define CORE_INCLUDE_IO_HW_H_

/*Inlcudes*/
#include "stdint.h"


/*Public defines*/
#define ADC_INPUT_PIN 2
/*Public enums*/



typedef enum INPUT_PINS {
	INPUT_1 = 12, /*GPIO12*/
	INPUT_2 = 13, /*GPIO13*/
}in_pin_t;

typedef enum OUTPUT_PINS {
	RELAY_1_OUT = 4 /*GPIO4*/,
	RELAY_2_OUT = 5 /*GPIO5*/,
}out_pin_t;

typedef enum PIN_STATE{
	CLEAR 	= 	0,
	SET		=	1
}pin_state_t;

/*Output interface */
void io_hw_init(void);

pin_state_t io_hw_get_pin(out_pin_t pin);

void io_hw_set_pin(out_pin_t pin,pin_state_t state);

#endif /* CORE_INCLUDE_IO_HW_H_ */
