#include "user_interface.h"
#include "../modules/drivers/include/uart.h"
#include "osapi.h"
#include "../modules/include/io_hw.h"
#include "../modules/include/secrets.h"
#include "../modules/include/temp_meas.h"
#include "../modules/include/wifi_app_Layer.h"
#include "../modules/include/mqtt_app_Layer.h"

#define MAIN_STATE_MACHINE_CYCLIC_TIME 50 /*ms*/
#define MAX_CYCLIC_TIME 2000 /*ms*/

#define GPIO_CYCLE_TIME 50 /*ms*/
#define ADC_CYCLE_TIME 100 /*ms*/
#define TEMPERATURE_MEASUREMENT_CYCLE_TIME 500 /*ms*/
#define DEBUG_UART_OUTPUT_CYCLE_TIME 1000 /*ms*/

typedef enum {
	SM_UART_INIT,
	SM_IO_HARDWARE_INIT,
	SM_TEMPERATURE_INIT,
	SM_WIFI_INIT,
	SM_MQTT_INIT,
	SM_END_OF_INIT
}init_phase_t;

static os_timer_t init_SM_timer;
static os_timer_t main_SM_timer;

static uint32_t sch_counter = 0;
static init_phase_t init_phase = SM_UART_INIT;


uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void) {
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
	case FLASH_SIZE_4M_MAP_256_256:
		rf_cal_sec = 128 - 8;
		break;

	case FLASH_SIZE_8M_MAP_512_512:
		rf_cal_sec = 256 - 5;
		break;

	case FLASH_SIZE_16M_MAP_512_512:
	case FLASH_SIZE_16M_MAP_1024_1024:
		rf_cal_sec = 512 - 5;
		break;

	case FLASH_SIZE_32M_MAP_512_512:
	case FLASH_SIZE_32M_MAP_1024_1024:
		rf_cal_sec = 1024 - 5;
		break;

	default:
		rf_cal_sec = 0;
		break;
	}

	return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void) {

}

static void ICACHE_FLASH_ATTR initialization_SM(void *arg)
{

	switch(init_phase)
	{
		case SM_UART_INIT:
			//Initialize UART
			os_printf("\r\nUart initializing ...\r\n");
			uart_init(BIT_RATE_115200, BIT_RATE_115200);
			init_phase = SM_IO_HARDWARE_INIT; /* next phase */
		break;

		case SM_IO_HARDWARE_INIT:
			//Initialize IO hardware
			os_printf("\r\nIO hardware ...\r\n");
			io_hw_init();
			init_phase = SM_TEMPERATURE_INIT; /* next phase */
		break;

		case SM_TEMPERATURE_INIT:
			//Initialize temperature measurement*/
			os_printf("\r\nTemperature measurement initialize ...\r\n");
			init_temp_meas();
			temp_meas_perform_conversion();
			init_phase = SM_WIFI_INIT; /* next phase */
		break;

		case SM_WIFI_INIT:
			if(WIFI_APPL_WIFI_INIT == wifi_app_get_wifi_conn_status ())
			{
				os_printf("\r\nWIFI initializing ...\r\n");
				/*Connect to WIFI*/
				wifi_app_wifi_connect();
			}
			else if(WIFI_APPL_WIFI_CONNECTING == wifi_app_get_wifi_conn_status ())
			{
				os_printf("\r\nWIFI connecting ...\r\n");
			}
			else if(WIFI_APPL_WIFI_CONNECTED == wifi_app_get_wifi_conn_status ())
			{
				os_printf("\r\nWIFI connected ...\r\n");
				init_phase = SM_MQTT_INIT; /* next phase */
			}
		break;

		case SM_MQTT_INIT:
			if(MQTT_APPL_INIT == mqtt_app_get_status())
			{
				mqtt_app_layer_init(); /* TODO Add check if mqtt init was sucesfull */
			}
			else if(MQTT_APPL_CONNECTING== mqtt_app_get_status())
			{
				os_printf("\r\nMQTT connecting ...\r\n");
			}
			else if(MQTT_APPL_CONNECTED == mqtt_app_get_status())
			{
				os_printf("\r\nMQTT connected ...\r\n");
				init_phase = SM_END_OF_INIT; /* next phase */
			}
		break;

		case SM_END_OF_INIT:
			os_printf("\r\nInitialization completed ...\r\n");
			mqtt_setter_availability(AVAILABLE);
			os_timer_disarm(&init_SM_timer);
			os_timer_arm(&main_SM_timer, MAIN_STATE_MACHINE_CYCLIC_TIME, 1);
		break;
	}

}

static void ICACHE_FLASH_ATTR main_SM(void *arg)
{
	/* Vars */
	static tm_result_t temperature;
	static uint16_t raw_adc;

	/* Update counter */
	sch_counter+=MAIN_STATE_MACHINE_CYCLIC_TIME;

	/*Handle inputs*/
	if(0 == (sch_counter % GPIO_CYCLE_TIME))
	{
		mqtt_setter_input(	io_hw_get_pin(INPUT_1),
							io_hw_get_pin(INPUT_2));
		process_send_state_machine();
	}
	/*Handle ADC*/
	if(0 == (sch_counter % ADC_CYCLE_TIME))
	{
		raw_adc = system_adc_read();
		mqtt_setter_adc(raw_adc);
	}
	/* Handle temperature measurement */
	if (0 == (sch_counter % TEMPERATURE_MEASUREMENT_CYCLE_TIME))
	{
		/*Temperature measurement*/
		temp_meas_read_temp();
		temperature = temp_meas_get_current_temp();
		temp_meas_perform_conversion();
		mqtt_setter_temperature(temperature.cur_temp_whole,temperature.cur_temp_frac);
	}
	/*Handle debug info */
	if (0 == (sch_counter % DEBUG_UART_OUTPUT_CYCLE_TIME))
	{
		os_printf("Inputs 1 :%d 2:%d\n",	io_hw_get_pin(INPUT_1),
											io_hw_get_pin(INPUT_2));
		os_printf("Current temperature:%d.%d \n",temperature.cur_temp_whole,temperature.cur_temp_frac);
		os_printf("ADC value:%d \n",raw_adc);
	}

	/* Wrap scheduler couter at MAX cyclic time */
	if(sch_counter == MAX_CYCLIC_TIME)
	{
		sch_counter = 0;
	}
}

/*
 * Entry point of user appliacation.
 * Initialize all components and start main state machine.
 */
void ICACHE_FLASH_ATTR user_init(void) {

	init_mqtt_topics();
	os_printf("\r\nSystem started ...\r\n");

	/* Prepare state machines clocks */
	os_timer_disarm(&init_SM_timer);
	os_timer_setfn(&init_SM_timer, (os_timer_func_t *)initialization_SM, (void *)0);

	os_timer_disarm(&main_SM_timer);
	os_timer_setfn(&main_SM_timer, (os_timer_func_t *)main_SM, (void *)0);

	/* Start initialization state machine */
	os_timer_arm(&init_SM_timer, 250, 1);
}
