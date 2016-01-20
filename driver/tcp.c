// by 0ff - http://www.esp8266.com/viewtopic.php?f=9&t=523#p2409
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"
#include "tcp.h"

#include "user_config.h"
#include "user_gpio.h"

LOCAL uint16_t server_timeover = 60; // 60 sec
LOCAL struct espconn masterconn;


LOCAL void ICACHE_FLASH_ATTR
shell_tcp_disconcb(void *arg) {
    struct espconn *pespconn = (struct espconn *) arg;

    os_printf("tcp connection disconnected\n");
}

LOCAL void ICACHE_FLASH_ATTR
shell_tcp_recvcb(void *arg, char *pusrdata, unsigned short length)
{
    struct espconn *pespconn = (struct espconn *) arg;


		if(os_strcmp(pusrdata, "relay-on\r\n")==0){
			my_gpio_output_set(GPIO_RELAY, 1);
		}else if(os_strcmp(pusrdata, "relay-off\r\n")==0){
			my_gpio_output_set(GPIO_RELAY, 0);
		}

    os_printf(">'%s' ", pusrdata);
		//espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));
    int i;
    for(i = 0; i < length; i++) {
        os_printf("0x%02X ", pusrdata[i]);
    }
    os_printf("\n");
    // espconn_sent(pespconn, pusrdata, length); //echo

}

LOCAL void ICACHE_FLASH_ATTR
tcpserver_connectcb(void *arg)
{
    struct espconn *pespconn = (struct espconn *)arg;

    os_printf("tcp connection established\n");

    espconn_regist_recvcb(pespconn, shell_tcp_recvcb);
    // espconn_regist_reconcb(pespconn, tcpserver_recon_cb);
    espconn_regist_disconcb(pespconn, shell_tcp_disconcb);
    // espconn_regist_sentcb(pespconn, tcpclient_sent_cb);
}

void ICACHE_FLASH_ATTR
shell_init(void)
{
    masterconn.type = ESPCONN_TCP;
    masterconn.state = ESPCONN_NONE;
    masterconn.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
    masterconn.proto.tcp->local_port = 23;
    espconn_regist_connectcb(&masterconn, tcpserver_connectcb);
    espconn_accept(&masterconn);
    espconn_regist_time(&masterconn, server_timeover, 0);

    os_printf("[%s] initializing shell!\n", __func__);
}
