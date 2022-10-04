/*******************************************************************************
* Title                 :   MQTT Topics configuration
* Filename              :   mqtt_topics_config.h
* Author                :   AKU
* Origin Date           :   4.4.2022
* Version               :   1.0.0
* Compiler              :   gcc
* Target                :   ESP8266
*******************************************************************************/
/******************************************************************************
 *  PUBLISH:
 *  	Temperature
 *  	Inputs
 *  	ADCs
 *  SUBSCRIBED:
 *  	Relays
 *
 * ****************************************************************************/

/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
*  4.4.2022    1.0         AKU      Module Created.
*  *
*******************************************************************************/
#ifndef MODULES_INCLUDE_MQTT_TOPICS_CONFIG_H_
#define MODULES_INCLUDE_MQTT_TOPICS_CONFIG_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "mqtt.h"
#include "mqtt_app_layer.h"
#include "io_hw.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Configuration Constants
*******************************************************************************/
#define TEMPERATURE_TOPIC 	"usd/test/temperature"
#define INPUT_1_TOPIC		"usd/test/input_1"
#define INPUT_2_TOPIC		"usd/test/input_2"
#define ADC_TOPIC			"usd/test/adc"
#define AVABILITY_TOPIC		"usd/test/availability"
#define OUTPUT_1_TOPIC		"usd/test/output_1_status"
#define OUTPUT_2_TOPIC		"usd/test/output_2_status"

#define RELAY_1_TOPIC		"usd/test/relay_1"
#define RELAY_2_TOPIC		"usd/test/relay_2"

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void init_mqtt_topics(void);

void mqtt_setter_input(pin_state_t input_1, pin_state_t input_2);
void mqtt_setter_temperature(int32_t whole, int32_t frac);
void mqtt_setter_adc(uint16_t adc_val);
void mqtt_setter_availability(availability_state_t avability);
void mqtt_setter_output1_state(pin_state_t output_1);
void mqtt_setter_output2_state(pin_state_t output_2);

#endif /*MODULES_INCLUDE_MQTT_TOPICS_CONFIG_H_*/

/*** End of File **************************************************************/
