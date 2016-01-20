
#ifndef __USER_GPIO_H__
#define __USER_GPIO_H__
#include "gpio.h"
#include "gpio16.h"

#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"


/* GPIO redeclarations from https://github.com/esp8266/esp8266-wiki/wiki/Pin-definition
for ESP-201 dev board.
*/
#define GPIO_DS18B20 2
#define BIT_DS18B20 BIT2

#define GPIO_RED 15
#define BIT_RED BIT15

#define GPIO_GREEN 13
#define BIT_GREEN BIT13

#define GPIO_BLUE 12
#define BIT_BLUE BIT12

#define GPIO_SPKR 5
#define BIT_SPKR BIT5

#define GPIO_WHITE 14
#define BIT_WHITE BIT14

#define GPIO_RELAY 16
#define BIT_RELAY BIT16

void ICACHE_FLASH_ATTR my_gpio_output_set(uint8 gpio, uint8 value);


#endif // __USER_GPIO_H__
