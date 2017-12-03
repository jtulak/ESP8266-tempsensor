#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"
#include "tcp.h"

// custom headers
#include "user_config.h"
#include "user_gpio.h"
#include "user_sleep.h"


#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static volatile os_timer_t some_timer;


// Print Ip Address
void print_ip(char *desc, uint32 addr)
{
    os_printf("%s", desc);
    if(addr>0)
    {
    os_printf("%d.%d.%d.%d", IP2STR(&addr));
    }
    else
      os_printf("Not set");
    os_printf("\n\r");
}


struct s_blink {
  uint8 red;
  uint8 green;
  uint8 blue;
  uint8 spkr;
};

struct s_blink blink = {1,0,0,0};

uint8 get_wifi_status(void){
  static uint8 last_con_status;
  struct ip_info ipinfo;
  uint8 wifi_status;

  wifi_status = wifi_station_get_connect_status();
  if(last_con_status != wifi_status){
    switch(wifi_status){
      case STATION_IDLE:
        os_printf("wifi idle\r\n");
        break;
      case STATION_CONNECTING:
        os_printf("wifi connecting\r\n");
        break;
      case STATION_WRONG_PASSWORD:
        os_printf("Wrong password for SSID %s!\r\n", SSID);
        break;
      case STATION_NO_AP_FOUND:
        os_printf("No AP with SSID %s was found!\r\n", SSID);
        break;
      case STATION_CONNECT_FAIL:
        os_printf("wifi connection failed!\r\n");
        break;
      case STATION_GOT_IP:
        blink.green = 3;
        blink.blue = 2;

        ipinfo.ip.addr=0;
        ipinfo.netmask.addr=255;
        ipinfo.gw.addr=0;
        wifi_get_ip_info(0x00, &ipinfo);
        os_printf("Connected!\r\n");
        print_ip("IP Address: ", ipinfo.ip.addr);
        print_ip("Netmask   : ", ipinfo.netmask.addr);
        print_ip("Gateway   : ", ipinfo.gw.addr);

        shell_init();
        sleep_init();
        //while(1) os_delay_us(300*1000);

        break;
    }
    last_con_status = wifi_status;
  }
  return last_con_status;
}



void some_timerfunc(void *arg)
{
  if (get_wifi_status() == STATION_GOT_IP){
    //sleep_init();
    sleep_wakeup();
  }
  //sleep_attempt();
  //sleep_init();

  if(blink.red){
    if(GPIO_INPUT_GET(GPIO_RED)){
      my_gpio_output_set(GPIO_RED,0);
      blink.red--;
    }else{
      my_gpio_output_set(GPIO_RED,1);
    }
  }
  if(blink.blue){
    if(GPIO_INPUT_GET(GPIO_BLUE)){
      my_gpio_output_set(GPIO_BLUE,0);
      blink.blue--;
    }else{
      my_gpio_output_set(GPIO_BLUE,1);
    }
  }
  if(blink.green){
    if(GPIO_INPUT_GET(GPIO_GREEN)){
      my_gpio_output_set(GPIO_GREEN,0);
      blink.green--;
    }else{
      my_gpio_output_set(GPIO_GREEN,1);
    }
  }
  if(blink.spkr){
    if(GPIO_INPUT_GET(GPIO_SPKR)){
      my_gpio_output_set(GPIO_SPKR,0);
      blink.spkr--;
    }else{
      my_gpio_output_set(GPIO_SPKR,1);
    }
  }
}


//Hello world task
static void ICACHE_FLASH_ATTR
task_hello(os_event_t *events)
{
  os_delay_us(10);
  //print text to the serial output
  os_printf("Hello World!\r\n");
}

//Init function
void ICACHE_FLASH_ATTR
user_init()
{
  char ssid[32] = SSID;
  char password[64] = SSID_PASSWORD;
  struct station_config stationConf;
  // Initialize UART0
  uart_div_modify(0, UART_CLK_FREQ / 9600);
  // init GPIO
  my_gpio_init();

  //Set station mode
  wifi_set_opmode(STATION_MODE);
  wifi_set_sleep_type(LIGHT_SLEEP_T);

  //Set ap settings
  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 64);
  wifi_station_set_config(&stationConf);

  //Set GPIO low
  my_gpio_output_set(GPIO_WHITE, 0);
  my_gpio_output_set(GPIO_RED, 0);
  my_gpio_output_set(GPIO_GREEN, 0);
  my_gpio_output_set(GPIO_BLUE, 0);
  my_gpio_output_set(GPIO_RELAY, 0);


  //Disarm timer
  os_timer_disarm(&some_timer);

  //Setup timer
  os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

  //Arm the timer
  //&some_timer is the pointer
  //1000 is the fire time in ms
  //0 for once and 1 for repeating
  os_timer_arm(&some_timer, 1000, 1);

  //Set up the hello world task
  system_os_task(task_hello, user_procTaskPrio, user_procTaskQueue, user_procTaskQueueLen);
  //send a message to the "hello world" task to activate it
  system_os_post(user_procTaskPrio, 0, 0 );
}
