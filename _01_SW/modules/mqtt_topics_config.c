/*
 * mqtt_topics_config.c
 *
 *  Created on: 05.04.2022
 *      Author: kulet
 */

#include "mqtt_topics_config.h"
#include "io_hw.h"
#include "temp_meas.h"
#include "osapi.h"

/* Callback function prototypes*/
static void MQTT_relay1_topic_callback(const char *payload);
static void MQTT_relay2_topic_callback(const char *payload);

/******************************************************************************
*  Subscribed topics objects
*******************************************************************************/

static MQTT_sub_msg mqtt_relay_1 =
{
	.topic = RELAY_1_TOPIC,
	.callback_fcn = MQTT_relay1_topic_callback,
};

static MQTT_sub_msg mqtt_relay_2 =
{
	.topic = RELAY_2_TOPIC,
	.callback_fcn = MQTT_relay2_topic_callback,
};


/******************************************************************************
*  Published topics objects
*******************************************************************************/
static MQTT_pub_msg mqtt_temperature =
{
		.cycle_time_factor = 1000,
		.topic = TEMPERATURE_TOPIC,
};

static MQTT_pub_msg mqtt_input1 =
{
		.cycle_time_factor = 1000,
		.topic = INPUT_1_TOPIC,
};

static MQTT_pub_msg mqtt_input2 =
{
		.cycle_time_factor = 1000,
		.topic = INPUT_2_TOPIC,
};

static MQTT_pub_msg mqtt_adc =
{
		.cycle_time_factor = 1000,
		.topic = ADC_TOPIC,
};

static MQTT_pub_msg mqtt_availability =
{
		.cycle_time_factor = 1000,
		.topic = AVABILITY_TOPIC,
};

static MQTT_pub_msg mqtt_output1_status =
{
		.cycle_time_factor = 1000,
		.topic = RELAY_1_TOPIC,
};

static MQTT_pub_msg mqtt_output2_status =
{
		.cycle_time_factor = 1000,
		.topic = RELAY_1_TOPIC,
};

/******************************************************************************
* Function to init mqtt topics
*******************************************************************************/
void init_mqtt_topics(void)
{
	/* Subscribe topics */
	init_sub_MQTT_msg(&mqtt_relay_1);
	init_sub_MQTT_msg(&mqtt_relay_2);
	/* Publish topics */
	init_pub_MQTT_msg(&mqtt_temperature);
	init_pub_MQTT_msg(&mqtt_input1);
	init_pub_MQTT_msg(&mqtt_input2);
	init_pub_MQTT_msg(&mqtt_adc);
	init_pub_MQTT_msg(&mqtt_availability);
	init_pub_MQTT_msg(&mqtt_output1_status);
	init_pub_MQTT_msg(&mqtt_output2_status);
	/* Init value */
	mqtt_setter_temperature(0,0);
	mqtt_setter_input(0,0);
	mqtt_setter_adc(0);
	mqtt_setter_availability(NOT_AVAILABLE);
	mqtt_setter_output_state(0,0);
}

/******************************************************************************
* Inputs processing
*******************************************************************************/
static void MQTT_relay1_topic_callback(const char *payload)
{
	if (os_strcmp(payload, "ON") == 0)
	{
		io_hw_set_pin(RELAY_1_OUT,1);
	}
	else if(os_strcmp(payload, "OFF") == 0)
	{
		io_hw_set_pin(RELAY_1_OUT,0);
	}

}

static void MQTT_relay2_topic_callback(const char *payload)
{
	if (os_strcmp(payload, "ON") == 0)
	{
		io_hw_set_pin(RELAY_2_OUT,1);
	}
	else if(os_strcmp(payload, "OFF") == 0)
	{
		io_hw_set_pin(RELAY_2_OUT,0);
	}

}

/******************************************************************************
* Setters functions
*******************************************************************************/
void mqtt_setter_temperature(int32_t whole, int32_t frac)
{
	os_sprintf(mqtt_temperature.payload,"{\"temperature\" : %d.%d }",whole,frac);
}

void mqtt_setter_input(pin_state_t input_1, pin_state_t input_2)
{
	if(0 == input_1)
	{
		os_sprintf(mqtt_input1.payload,"{\"state\" : \"OFF\"}");
	}
	else
	{
		os_sprintf(mqtt_input1.payload,"{\"state\" : \"ON\"}");
	}

	if(0 == input_2)
	{
		os_sprintf(mqtt_input2.payload,"{\"state\" : \"OFF\"}");
	}
	else
	{
		os_sprintf(mqtt_input2.payload,"{\"state\" : \"ON\"}");
	}
}

void mqtt_setter_adc(uint16_t adc_val)
{
	os_sprintf(mqtt_adc.payload,"{\"adc\" : \"%d\"}",adc_val);
}

void mqtt_setter_availability(availability_state_t avability)
{
	if(NOT_AVAILABLE == avability)
	{
		os_sprintf(mqtt_availability.payload,"{\"availability\" : \"offline\"}");
	}
	else
	{
		os_sprintf(mqtt_availability.payload,"{\"availability\" : \"online\"}");
	}

}

void mqtt_setter_output_state(pin_state_t output_1, pin_state_t output_2)
{
	if(CLEAR == output_1)
	{
		os_sprintf(mqtt_input1.payload,"{\"state\" : \"OFF\"}");
	}
	else
	{
		os_sprintf(mqtt_input1.payload,"{\"state\" : \"ON\"}");
	}

	if(CLEAR == output_2)
	{
		os_sprintf(mqtt_input2.payload,"{\"state\" : \"OFF\"}");
	}
	else
	{
		os_sprintf(mqtt_input2.payload,"{\"state\" : \"ON\"}");
	}
}

