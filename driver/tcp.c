// by 0ff - http://www.esp8266.com/viewtopic.php?f=9&t=523#p2409
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"
#include "tcp.h"


#include "ds18b20.h"
#include "user_config.h"
#include "user_gpio.h"

LOCAL uint16_t server_timeover = 60; // 60 sec
LOCAL struct espconn masterconn;

enum {
  GET_NOT_HTTP,
  GET_404,
  GET_INDEX,
  GET_RELAY_ON,
  GET_RELAY_OFF,
};

uint8 parse_header(char*msg){
  uint32 i;
  char req[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // max length is 20
  if (msg[0]=='G' && msg[1]=='E' && msg[2]=='T'){
    // until first space
    for (i=0; msg[4+i] != ' '; i++) {
      if(i >= 20)
        return GET_404;
      req[i] = msg[4+i];
    }
    os_printf("GET: %s\r\n",msg);
    if(os_strcmp(req, "/") == 0){
      return GET_INDEX;
    }
    if(os_strcmp(req, "/relay-on") == 0){
      return GET_RELAY_ON;
    }
    if(os_strcmp(req, "/relay-off") == 0){
      return GET_RELAY_OFF;
    }
    return GET_404;
  }
  return GET_NOT_HTTP;
}


LOCAL void ICACHE_FLASH_ATTR
shell_tcp_disconcb(void *arg) {
    struct espconn *pespconn = (struct espconn *) arg;

    os_printf("tcp connection disconnected\n");
}

LOCAL void ICACHE_FLASH_ATTR
shell_tcp_recvcb(void *arg, char *pusrdata, unsigned short length)
{
    struct espconn *pespconn = (struct espconn *) arg;
    uint8 req = parse_header(pusrdata);
    if (req == GET_NOT_HTTP){
      os_printf("Not HTTP: ");
      if(os_strcmp(pusrdata, "relay-on\r\n")==0){
  			my_gpio_output_set(GPIO_RELAY, 1);
        os_printf("set relay on");
  		}else if(os_strcmp(pusrdata, "relay-off\r\n")==0){
  			my_gpio_output_set(GPIO_RELAY, 0);
        os_printf("set relay off");
  		}else if(os_strcmp(pusrdata, "temperature\r\n")==0){
        char temps[5][DS_REPLY_LENGTH];
        read_all_temps(temps);
        int i;
        for (i=0; i<DS_MAX_PROBES; i++){
          if (os_strlen(temps[i]) == 0)
            continue;
          espconn_sent(pespconn, (uint8*)temps[i], os_strlen(temps[i]));
          espconn_sent(pespconn, (uint8*)"\r\n", os_strlen("\r\n"));
        }
  		}


      os_printf(">'%s' ", pusrdata);
  		//espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));
      /*int i;
      for(i = 0; i < length; i++) {
          os_printf("0x%02X ", pusrdata[i]);
      }*/
      os_printf("\n");
    } else {
      os_printf("HTTP request: ");

      if(req == GET_404){
        os_printf("404\r\n");
        pusrdata ="HTTP/1.0 404 Not Found\n";
        espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));

      }else if (req == GET_RELAY_ON){
        pusrdata ="HTTP/1.0 200 OK\n";
        espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));
  			my_gpio_output_set(GPIO_RELAY, 1);
        os_printf("set relay on\r\n");

      }else if (req == GET_RELAY_OFF){
        pusrdata ="HTTP/1.0 200 OK\n";
        espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));
  			my_gpio_output_set(GPIO_RELAY, 0);
        os_printf("set relay off\r\n");

      }else if (req == GET_INDEX){
        os_printf("get index\r\n");
        char temps[DS_MAX_PROBES][DS_REPLY_LENGTH];
        int i,j;
        for (i=0; i<DS_MAX_PROBES; i++)
          for (j=0; j<DS_REPLY_LENGTH; j++)
            temps[i][j] = 0;

        read_all_temps(temps);
        os_sprintf(pusrdata, "HTTP/1.0 200 OK\nContent-Type: text/html; charset=UTF-8\nConnection: close\n\n%s\n%s\n%s\n%s\n%s\n", temps[0], temps[1], temps[2], temps[3], temps[4]);
        espconn_sent(pespconn, (uint8*)pusrdata, os_strlen(pusrdata));
      }

      espconn_disconnect(pespconn);
    }

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
    masterconn.proto.tcp->local_port = 80;
    espconn_regist_connectcb(&masterconn, tcpserver_connectcb);
    espconn_accept(&masterconn);
    espconn_regist_time(&masterconn, server_timeover, 0);

    os_printf("[%s] initializing shell!\n", __func__);
}
