#ifndef INCLUDE_LOGUTIL_H_
#define INCLUDE_LOGUTIL_H_

#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqtt.h"
#include "wifi.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "espconn.h"

#define MAX_LOG_LENGHT 128
const int LOG_ADDR[4] = { 192, 168, 0, 157 };
const int LOG_PORT = 6685;

//Global variables
struct espconn G_LOG_UDP_CONN;
esp_udp G_LOG_SOCKET;

enum eResult {
	R_FAIL = 0, R_PASS = 1
};

enum eResult ePrepareUDPSocket(const unsigned ipAddr[4], const unsigned port);

enum eResult LOG_eCloseUDPSocket();

enum eResult LOG(char sMessage[], int var);

#endif /* INCLUDE_MQTT_APPLAYER_H_ */
