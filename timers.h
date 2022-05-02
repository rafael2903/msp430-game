#ifndef TIMERS_H_
#define TIMERS_H_

#include <msp430.h>

#define _1_8_SECOND 4096
#define _1_4_SECOND 8192
#define _1_2_SECOND 16384
#define _1_SECOND   32768
#define _2_SECONDS  65536

void config_timers();
void delay(unsigned int time);
void start_buzzer();
void stop_buzzer();

#endif  // TIMERS_H_
