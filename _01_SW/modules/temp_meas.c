/*
 * temp_meas.c
 *
 *  Created on: 24.02.2022
 *      Author: kulet
 */

#include "include/temp_meas.h"

#include "drivers/include/ds18b20.h"
#include "stdint.h"
#include "debug.h"


static tm_result_t result = {0};
static int8_t temp_calibration	=	0;
static uint8_t address[8] = {0};


func_result init_temp_meas(){
	int32_t search_result;
	uint8_t data[12];

	/*Init DS library*/
	ds_init();
	/* Search sensor*/
	search_result = ds_search(address);
		if(search_result)
		{
			INFO("Found Device @ %02x %02x %02x %02x %02x %02x %02x %02x\r\n", address[0], address[1], address[2], address[3], address[4], address[5], address[6], address[7]);
			return SUCESS;
		}
		else {
			INFO("No DS18B20 detected, sorry.\r\n");
			return ERROR;
		}
}

void temp_meas_perform_conversion(){
	// perform the conversion, call AFTER init!
	ds_reset();
	ds_select(address);
	ds_write(DS1820_CONVERT_T, 1); // perform temperature conversion
}

void temp_meas_read_temp(void){

	uint32_t i;
	int32_t HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
	uint8_t data[12];

	ds_reset();
	ds_select(address);
	ds_write(DS1820_READ_SCRATCHPAD, 0); // read scratchpad

	for(i = 0; i < 9; i++)
	{
		data[i] = ds_read();
	}


	LowByte = data[0];
	HighByte = data[1];
	TReading = (HighByte << 8) + LowByte;
	SignBit = TReading & 0x8000;  // test most sig bit
	if (SignBit) // negative
		TReading = (TReading ^ 0xffff) + 1; // 2's comp

	Whole = TReading >> 4;  // separate off the whole and fractional portions
	Fract = (TReading & 0xf) * 100 / 16;

	/* Update internal var*/
	result.cur_temp_whole = Whole;
	result.cur_temp_frac = Fract;
}

tm_result_t temp_meas_get_current_temp(void){
	tm_result_t retVal;
	retVal.cur_temp_whole = result.cur_temp_whole + temp_calibration;
	retVal.cur_temp_frac = result.cur_temp_frac;
	return retVal;
}

void set_calibration(uint8_t calibration)
{
	temp_calibration = calibration;
}
