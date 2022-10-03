/*
 * temp_meas.h
 *
 *  Created on: 24.02.2022
 *      Author: kulet
 */

#ifndef USER_INCLUDE_TEMP_MEAS_H_
#define USER_INCLUDE_TEMP_MEAS_H_
#endif /* USER_INCLUDE_TEMP_MEAS_H_ */

#include "../modules/include/common.h"
#include <stdint.h>

/* Struct to hold temperature measurement*/
typedef struct tm_result_t{
	uint8_t cur_temp_whole; /*Whole part*/
	uint8_t cur_temp_frac;	/*Fraction part*/
}tm_result_t;

/*External interface */
/* Fun to init measurement*/
func_result init_temp_meas(void);
/* Fun to send command to perform conversion*/
void temp_meas_perform_conversion(void);
/* Fun to read measurement and copy it to static buffer*/
void temp_meas_read_temp(void);
/* Static buffer getter*/
tm_result_t temp_meas_get_current_temp(void);
/* Setter of calib value*/
void set_calibration(uint8_t calibration);
