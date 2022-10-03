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


/******************************************************************************
* Includes
*******************************************************************************/
#include "include/user_config.h"
#include "mqtt.h"
#include "osapi.h"
#include "mqtt_app_layer.h"
#include "debug.h"
#include "secrets.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MAX_SEND_SCH_CYCLE_TIME 5*1000 /* 30s */
#define SEND_SCH_CYCLE_TIME	50
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static MQTT_Client mqtt_client;
static mqtt_status_t current_status = MQTT_APPL_INIT;
static MQTT_SubscribedArray_t sub_topics;
static MQTT_PublishArray_t	pub_topics;
static uint32_t sch_counter;
//static MQTT_msg srrOutput;

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void mqtt_app_ConnectedCb(uint32_t *args);
static void mqtt_app_DataCb(uint32_t *args, const char* topic, uint32_t topic_len,
		const char *data, uint32_t data_len);
static void mqtt_app_PublishedCb(uint32_t *args);
static void mqtt_app_DisconnectedCb(uint32_t *args);

/******************************************************************************
* Function Definitions
*
*******************************************************************************/
void mqtt_app_layer_init(void){
	uint32_t iter;
	char mqtt_username_decoded[SECRET_MQTT_USERNAME_LEN + 1];
	char mqtt_pass_decoded[SECRET_MQTT_PASS_LEN + 1];

	sec_get_secret_as_string(mqtt_username_decoded,SECRET_MQTT_USERNAME,SECRET_MQTT_USERNAME_LEN);
	sec_get_secret_as_string(mqtt_pass_decoded,SECRET_MQTT_PASS,SECRET_MQTT_PASS_LEN);

	//Prepare connection data
	MQTT_InitConnection(&mqtt_client,MQTT_HOST,MQTT_PORT, 0);
	//Prepare client data
	MQTT_InitClient(&mqtt_client, "ESP1",mqtt_username_decoded,mqtt_pass_decoded,MQTT_KEEPALIVE, 1);

	/*Register callbacks */
	MQTT_OnConnected(&mqtt_client, mqtt_app_ConnectedCb);
	MQTT_OnDisconnected(&mqtt_client, mqtt_app_DisconnectedCb);
	MQTT_OnPublished(&mqtt_client, mqtt_app_PublishedCb);
	MQTT_OnData(&mqtt_client, mqtt_app_DataCb);

	/* Connect */
	current_status = MQTT_APPL_CONNECTING;
	MQTT_Connect(&mqtt_client);

	/* Subcribe topics */
	for(iter = 0; iter < MAX_SUB_TOPICS ; iter++)
	{
		MQTT_Subscribe(&mqtt_client,sub_topics.array[iter]->topic,0);
	}

}

void init_sub_MQTT_msg(MQTT_sub_msg *msg) {

	uint32_t topic_len;

	if (sub_topics.counter <= MAX_SUB_TOPICS) { //check is array have space
			sub_topics.array[sub_topics.counter] = msg;
			sub_topics.counter++;
		}
}

void init_pub_MQTT_msg(MQTT_pub_msg *msg)
{
	if (pub_topics.counter <= MAX_PUB_TOPICS) { //check is array have space
			pub_topics.array[pub_topics.counter] = msg;
			pub_topics.counter++;
		}
}

void process_send_state_machine(void)
{
	uint8_t index;
	uint32_t length;

	/* Update counter */
	sch_counter+= SEND_SCH_CYCLE_TIME;

	for(index = 0; index < MAX_PUB_TOPICS ; index++)
	{
		if(0 == (sch_counter % pub_topics.array[index]->cycle_time_factor))
		{
			length=os_strlen(pub_topics.array[index]->payload);
			MQTT_Publish(	&mqtt_client,
							pub_topics.array[index]->topic,
							pub_topics.array[index]->payload,
							length,
							0,
							0);
		}
	}

	/* Wrap scheduler couter at MAX cyclic time */
	if(sch_counter > MAX_SEND_SCH_CYCLE_TIME*2)
	{
		sch_counter = 0;
	}
}

mqtt_status_t mqtt_app_get_status(void)
{
	return current_status;
}

MQTT_Client* mqtt_app_get_client(void)
{
	return &mqtt_client;
}


static void mqtt_app_DisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	os_printf("MQTT: Disconnected\r\n");
}

static void mqtt_app_PublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	os_printf("MQTT: Published\r\n");
}

static void mqtt_app_DataCb(uint32_t *args, const char* topic, uint32_t topic_len,
		const char *data, uint32_t data_len) {

	uint8_t i;
	static char topicBuf[MAX_TOPIC_LENGTH];
	static char dataBuf[MAX_PAYLOAD_LENGTH];

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	for (i = 0; i < MAX_SUB_TOPICS; i++) {
		/* Look for subscribed topic */
		os_printf("%s %s",sub_topics.array[i]->topic,topicBuf);
		if (os_strcmp(sub_topics.array[i]->topic, topicBuf) == 0)
		{
			sub_topics.array[i]->callback_fcn(dataBuf);
		}
	}
}

static void mqtt_app_ConnectedCb(uint32_t *args)
{
	current_status = MQTT_APPL_CONNECTED;
}
/*************** END OF FUNCTIONS ***************************************************************************/
