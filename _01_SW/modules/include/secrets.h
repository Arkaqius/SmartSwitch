/*
 * secrets.h
 *
 *  Created on: 21.03.2022
 *      Author: kulet
 */

#ifndef CORE_INCLUDE_SECRETS_H_
#define CORE_INCLUDE_SECRETS_H_
#include "c_types.h"


#define MAX_SECRET_LEN	64

/* Globals secrets*/
extern const uint8_t SECRET_STA_SSID_LEN;
extern const uint8_t SECRET_STA_SSID[MAX_SECRET_LEN];
extern const uint8_t SECRET_STA_PASS_LEN;
extern const uint8_t SECRET_STA_PASS[MAX_SECRET_LEN];
extern const uint8_t SECRET_MQTT_PASS_LEN;
extern const uint8_t SECRET_MQTT_PASS[MAX_SECRET_LEN];
extern const uint8_t SECRET_MQTT_USERNAME_LEN;
extern const uint8_t SECRET_MQTT_USERNAME[MAX_SECRET_LEN];

void sec_get_secret_as_string(char* const output,const char* const input_secret, int32_t size);

#endif /* CORE_INCLUDE_SECRETS_H_ */
