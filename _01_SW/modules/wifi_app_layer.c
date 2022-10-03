/*******************************************************************************
* Title                 :   Wifi Application layer
* Filename              :   wifi_app_layer.c
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
#include "include/wifi_app_layer.h"
#include "user_interface.h"
#include "include/secrets.h"
#include "include/user_config.h"
#include "drivers/include/wifi.h"
#include "osapi.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static wifi_status_t current_status = WIFI_APPL_WIFI_INIT;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void wifiConnectCb(uint8_t status);
/******************************************************************************
* Function Definitions
*
*******************************************************************************/

void wifi_app_wifi_connect(void)
{
	char ssid_decoded[SECRET_STA_SSID_LEN + 1];
	char password_decoded[SECRET_STA_PASS_LEN + 1];

	sec_get_secret_as_string(ssid_decoded,SECRET_STA_SSID,SECRET_STA_SSID_LEN);
	sec_get_secret_as_string(password_decoded,SECRET_STA_PASS,SECRET_STA_PASS_LEN);

	/* Update status register*/
	current_status = WIFI_APPL_WIFI_CONNECTING;
	WIFI_Connect(ssid_decoded, password_decoded, wifiConnectCb);
}

static void wifiConnectCb(uint8_t status)
{
	if(STATION_GOT_IP == status)
	{
		current_status = WIFI_APPL_WIFI_CONNECTED;
	}
	else
	{
		current_status = WIFI_APPL_WIFI_ERROR;
	}
}

wifi_status_t wifi_app_get_wifi_conn_status(void){
	return current_status;
}

/*************** END OF FUNCTIONS ***************************************************************************/
