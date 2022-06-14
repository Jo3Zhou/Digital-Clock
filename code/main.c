#include <stdio.h>
#include "oled.h"
#include "sleep.h"
#include "display_time.h"
#include "buzz.h"
#include "stop_watch.h"
#include "deb_irq.h"
#include "state.h"

int main(){
	oled_initialize();
	oled_print_welcome();
	InterruptSystemSetup();
	SetupInterrput();
	set_defalut_time();
	state = state_idle;

	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x13);
	ControlWrite(&Spi, 0xb3);
	oled_print(&Spi,"time");
	XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x01);

	while(1){
		switch (state){
			case state_idle:
				dis_time();
				break;
			case state_change_time:
				dis_time();
				break;
			case state_stopwatch:
				dis_sw_time();
				break;
			case state_timer:
				dis_timer();
				break;
			case state_alarm:
				dis_alarm();
				break;
			default:
				dis_time();
				break;
		}

	}

	oled_off();
}
