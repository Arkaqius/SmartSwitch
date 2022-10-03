#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_


/*MQTT CONFIGURATIONS*/
#define MQTT_HOST			"192.168.0.3" //or "mqtt.yourdomain.com"
#define MQTT_PORT			1883
#define MQTT_BUF_SIZE		1024
#define MQTT_KEEPALIVE		120	 /*second*/
#define MQTT_CLIENT_ID		"DVES_%08X"
#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/
#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE	2048
#define PROTOCOL_NAMEv31	/*MQTT version 3.1 compatible with Mosquitto v0.15*/

/* MQTT topics config */
#define MAX_SUB_TOPICS 		2
#define MAX_PUB_TOPICS		6
#define MAX_TOPIC_LENGTH 	32
#define MAX_PAYLOAD_LENGTH 	128

/* WIFI CONFIGURAIONT*/
#define STA_TYPE AUTH_WPA2_PSK
#endif
