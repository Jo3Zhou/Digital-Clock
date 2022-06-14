#ifndef STOP_WATCH_H
#define STOP_WATCH_H

#include "stopwatch.h"
#include "xparameters.h"
#include "xil_io.h"
#include "stdio.h"

void sw_on();
void sw_stp();
void sw_clr();
u8 sw_sec_out();
u8 sw_min_out();
u8 sw_hor_out();


#endif
