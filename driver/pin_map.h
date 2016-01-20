/*
 * There is not a 1:1 mapping between physical pin numbers numbers and the
 * various constants uses to control GPIO functionality.
 *
 * The following globals are used to make this easy without adding masses of
 * overhead that a fully fledged abstraction library (such as that provided by
 * Arduino) might add:
 *
 * pin_mux[gpio]   ->> returns the correct constant for gpio 'pin' to use
 *                     as the first argument to PIN_FUNC_SELECT(),
 *                     PIN_PULLDWN_DIS(), PIN_PULLUP_DIS(), PIN_PULLUP_EN(),
 *                     PIN_PULLDWN_DIS() functions
 *
 * pin_func[gpio]  ->> returns the correct constant for gpio 'gpio' to use
 *                     as the second argument to PIN_FUNC_SELECT()
 *
 * The gpio is passed directly to GPIO_DIS_OUTPUT(), GPIO_OUTPUT_SET() and
 * GPIO_OUTPUT_GET()
 *
 * pin_int_type[gpio] is used to return the GPIO_PIN_INTR_DISABLE state when
 * GPIO interrupts are enabled.
 *
 * The actual mappings are described in the file pin_map.c
 *
 * Not all gpio numbers are valid. This can be checked first using the function
 * is_valid_gpio_pin() which also checks for the gpio in the range
 * 0..GPIO_PIN_NUM-1
 *
 * Invalid gpio will also report -1 for pin_mux[] and pin_func[]
 *
 * Note that the ESP SDK provides an additional macro, GPIO_ID_PIN() which in
 * theory should be wrapped around all 'gpio' numbers passed to the register
 * functions, however, this macro essentially resolves to a NO-OP so code will
 * work fine without it. It is not inconceivable however that there could be
 * a future hardware version increment that breaks this, so I guess omit this
 * at your own risk.
 *
 * EXAMPLE
 * -------
 *
 *   uint8_t gpio = argv[2];
 *   if (!is_valid_gpio_pin(gpio)) { printf("error...\n"); return; }
 *   PIN_FUNC_SELECT(pin_mux[gpio], pin_func[gpio]);
 *   PIN_PULLDWN_DIS(pin_mux[gpio]);
 *   GPIO_DIS_OUTPUT(GPIO_ID_PIN(gpio));
 *
 */
#ifndef __PIN_MAP_H__
#define __PIN_MAP_H__

#include "c_types.h"
#include "user_config.h"
#include "gpio.h"

#define GPIO_PIN_NUM 17
#define GPIO_PIN_FUNC_INVALID (uint8_t)(-1)
#define GPIO_PIN_MUX_INVALID (uint32_t)(-1)

bool is_valid_gpio_pin(int gpio){
	if (gpio > 0 && gpio < 16){
		return true;
	}
	return false;
}

uint8 pin_func[GPIO_PIN_NUM] = {
	 FUNC_GPIO0
	,FUNC_GPIO1
	,FUNC_GPIO2
	,FUNC_GPIO3
	,FUNC_GPIO4
	,FUNC_GPIO5
	,GPIO_PIN_FUNC_INVALID//FUNC_GPIO6
	,GPIO_PIN_FUNC_INVALID//FUNC_GPIO7
	,GPIO_PIN_FUNC_INVALID//FUNC_GPIO8
	,FUNC_GPIO9
	,FUNC_GPIO10
	,GPIO_PIN_FUNC_INVALID//FUNC_GPIO11
	,FUNC_GPIO12
	,FUNC_GPIO13
	,FUNC_GPIO14
	,FUNC_GPIO15
	,GPIO_PIN_FUNC_INVALID//FUNC_GPIO16
};

// https://github.com/esp8266/esp8266-wiki/wiki/Pin-definition
uint32_t pin_mux[GPIO_PIN_NUM] = {
	 PERIPHS_IO_MUX_GPIO0_U
	,PERIPHS_IO_MUX_U0TXD_U
	,PERIPHS_IO_MUX_GPIO2_U
	,PERIPHS_IO_MUX_U0RXD_U
	,PERIPHS_IO_MUX_GPIO4_U
	,PERIPHS_IO_MUX_GPIO5_U
	,GPIO_PIN_MUX_INVALID//FUNC_GPIO6
	,GPIO_PIN_MUX_INVALID//FUNC_GPIO7
	,GPIO_PIN_MUX_INVALID//FUNC_GPIO8
	,GPIO_PIN_MUX_INVALID//PERIPHS_IO_MUX_GPIO5_U
	,GPIO_PIN_MUX_INVALID//PERIPHS_IO_MUX_GPIO5_U
	,GPIO_PIN_MUX_INVALID//FUNC_GPIO11
	,PERIPHS_IO_MUX_MTDI_U
	,PERIPHS_IO_MUX_MTCK_U
	,PERIPHS_IO_MUX_MTMS_U
	,PERIPHS_IO_MUX_MTDO_U
	,GPIO_PIN_MUX_INVALID//PERIPHS_IO_MUX_XPD_DCDC_U//FUNC_GPIO16
};

#ifdef GPIO_INTERRUPT_ENABLE
extern GPIO_INT_TYPE pin_int_type[GPIO_PIN_NUM];
#endif
#endif // #ifndef __PIN_MAP_H__
