#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "gpio.h"
#include "gpio16.h"
#include "ds18b20.h"
#include "user_config.h"
#include "user_gpio.h"


void ICACHE_FLASH_ATTR my_gpio_init(){
	// Initialize the GPIO subsystem.
  gpio_init();
  gpio16_output_conf();
}

// To simplify my_gpio_output_set, make here this macro.
// 'value' is a variable in the function.
#define _MY_GPIO_OUTPUT_SET(mux, func, bit) PIN_FUNC_SELECT(mux, func); if (value){gpio_output_set(bit, 0, bit, 0);} else {gpio_output_set(0, bit, bit, 0);}

void ICACHE_FLASH_ATTR my_gpio_output_set(uint8 gpio, uint8 value){
	switch(gpio){
		case GPIO_SPKR:
			_MY_GPIO_OUTPUT_SET(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5, BIT5)
			break;
		case GPIO_WHITE:
			_MY_GPIO_OUTPUT_SET(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14, BIT14)
			break;
		case GPIO_BLUE:
			_MY_GPIO_OUTPUT_SET(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO12, BIT12)
			break;
		case GPIO_GREEN:
			_MY_GPIO_OUTPUT_SET(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO13, BIT13)
			break;
		case GPIO_RED:
			_MY_GPIO_OUTPUT_SET(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15, BIT15)
			break;
		case GPIO_RELAY:
			gpio16_output_set(value);
			break;
	}
}
