#ifndef DISPLAY_TIME_H
#define DISPLAY_TIME_H

#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "timer.h"
#include "oled.h"
#include "timer_counter.h"

void dis_time();
void set_defalut_time();
void en_sec();
void en_min();
void en_hor();

void dis_sw_time();

void dis_timer();

void dis_alarm();

#endif
