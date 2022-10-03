/*******************************************************************************
* Title                 :   MQTT Application layer
* Filename              :   mqtt_app_layer.c
* Author                :   AKU
* Origin Date           :   10.03.2022
* Version               :   1.0.0
* Compiler              :   gcc
* Target                :   ESP8266
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description
*  10.03.2022    1.0         AKU      Module Created.
*
*******************************************************************************/
#ifndef CORE_INCLUDE_MQTT_APP_LAYER_H_
#define CORE_INCLUDE_MQTT_APP_LAYER_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "mqtt.h"
#include "user_config.h"
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/



/******************************************************************************
* Typedefs
*******************************************************************************/
typedef enum {	MQTT_APPL_ERROR 		= -1,
				MQTT_APPL_INIT 			= 0,
				MQTT_APPL_CONNECTING 	= 1,
				MQTT_APPL_CONNECTED 	= 2
}mqtt_status_t;

typedef struct {
	uint8_t topic[MAX_TOPIC_LENGTH];
	void (*callback_fcn)(const char *payload);
} MQTT_sub_msg;

typedef struct {
	uint32_t cycle_time_factor;
	uint8_t topic[MAX_TOPIC_LENGTH];
	uint8_t payload[MAX_PAYLOAD_LENGTH];
} MQTT_pub_msg;

typedef struct {
	uint32_t counter;
	MQTT_sub_msg* array[MAX_SUB_TOPICS];
} MQTT_SubscribedArray_t;

typedef struct {
	uint32_t counter;
	MQTT_pub_msg* array[MAX_PUB_TOPICS];
} MQTT_PublishArray_t;

typedef enum {
	NOT_AVAILABLE,
	AVAILABLE
}availability_state_t;

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Module init */
void mqtt_app_layer_init(void);
/* Return unit current status */
mqtt_status_t mqtt_app_get_status(void);
/* Return handler to mqtt clinet*/
MQTT_Client* mqtt_app_get_client(void);
/* Init MQTT subsribed message */
void init_sub_MQTT_msg(MQTT_sub_msg *msg);
/* Init MQTT publish message */
void init_pub_MQTT_msg(MQTT_pub_msg *msg);
/* Process send state machine */
void process_send_state_machine(void);

#endif /*CORE_INCLUDE_MQTT_APP_LAYER_H_*/

/*** End of File **************************************************************/
