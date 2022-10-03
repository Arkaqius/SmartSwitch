/*
 * secrets.c
 *
 *  Created on: 21.03.2022
 *      Author: kulet
 */

/*Includes*/
#include "include/secrets.h"

#include "c_types.h"
#include "osapi.h"

extern const uint8_t XOR_MAGIC_NUMBER;

void sec_get_secret_as_string(char* const output,const char* const input_secret, int32_t size)
{
	int32_t i;
	for(i = 0 ; i < size ; i++)
	{
		output[i] = input_secret[i] ^ XOR_MAGIC_NUMBER;
	}

	output[i] = 0x00;
}
