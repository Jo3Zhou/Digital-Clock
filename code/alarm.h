#ifndef ALARM_H
#define ALARM_H

#include "xparameters.h"
#include "xgpio.h"
#include "timer.h"
#include "oled.h"
#include "timer_counter.h"

//enable alarm
void en_alarm();
//cancel alarm
void cancel_alarm();
//set alarm min + 1
void en_alarm_min();
//set alarm hor + 1
void en_alarm_hor();

#endif
