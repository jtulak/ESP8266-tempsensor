#ifndef _USER_SLEEP_H_
#define _USER_SLEEP_H_


// Define used sleep mode
//#define SLEEP_MODE NONE_SLEEP_T
//#define SLEEP_MODE MODEM_SLEEP_T
#define SLEEP_MODE LIGHT_SLEEP_T

void sleep_loop(void);
void sleep_init(void);

#endif // _USER_SLEEP_H_
