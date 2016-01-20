/*
 Basic TCP framework: 0ff - http://www.esp8266.com/viewtopic.php?f=9&t=523#p2409
 License uncertain, but from context, Public Domain is most likely.
 HTTP and application parts added by Jan Tulak.
*/
#ifndef __TCP_H__
#define __TCP_H__

LOCAL void ICACHE_FLASH_ATTR shell_tcp_disconcb(void *arg);

LOCAL void ICACHE_FLASH_ATTR
shell_tcp_recvcb(void *arg, char *pusrdata, unsigned short length);

LOCAL void ICACHE_FLASH_ATTR tcpserver_connectcb(void *arg);

void ICACHE_FLASH_ATTR shell_init(void);

#endif // __TCP_H__
