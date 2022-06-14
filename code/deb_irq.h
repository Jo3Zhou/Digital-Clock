#ifndef DEB_IRQ_H
#define DEB_IRQ_H

#include <stdio.h>
#include "xscugic.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

//---------------------button interrput-------------------
#define IRQ_F2P0	61
#define IRQ_F2P1	62
#define IRQ_F2P2	63
#define IRQ_F2P3	64
#define IRQ_F2P4	65
#define IRQ_F2P5	66
#define IRQ_F2P6	67
#define IRQ_F2P7	68
#define IRQ_F2P8	84
#define IRQ_F2P9	85
//timer interrupt
#define IRQ_F2P10	86
//alarm interrupt
#define IRQ_F2P11	87

#define INT_TYPE_RISING_EDGE	0x03

XScuGic GicInst;

int InterruptSystemSetup();
int SetupInterrput();

#endif
