#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "user_interface.h"
#include "user_sleep.h"

bool CAN_SLEEP = false;
bool SLEEPING = false;


void sleep_init(void){
	os_printf("Configuring sleep.\r\n");
  // set up sleep
	CAN_SLEEP = true;
}

void sleep_wakeup(void)
{
	if(SLEEPING = true)
	{
		SLEEPING=false;
		CAN_SLEEP = true;
		os_printf("woke up\r\n");
		wifi_fpm_close();   //disable sleep function
		wifi_set_opmode(STATION_MODE);      //set wifi mode to station mode
		wifi_station_connect();         //connect ap
		os_printf("restored\r\n\r\n\r\n");
	}
}

void sleep_attempt(void)
{
  if (CAN_SLEEP){
		CAN_SLEEP = false;
		SLEEPING = true;
		os_printf("Can sleep\r\n");
		//gpio_pin_wakeup_enable(GPIO_ID_PIN(0), 5/*GPIO_PIN_INTR_HILEVEL*/);
	  wifi_set_sleep_type(LIGHT_SLEEP_T);
		wifi_station_disconnect();
		wifi_set_opmode_current(NULL_MODE);
		wifi_fpm_open();
		wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1);

		wifi_fpm_do_sleep(30*1000);
		//os_delay_us(300*1000);
		//system_deep_sleep(2000*1000*1000);

  }else{
		os_printf("Can NOT sleep\r\n");
	}
}
