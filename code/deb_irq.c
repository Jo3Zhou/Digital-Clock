#include "deb_irq.h"
#include "timer.h"
#include "state.h"
#include "display_time.h"
#include "stop_watch.h"
#include "buzz.h"
#include "alarm.h"

void IRQ_F2P0_Handler(void *CallBackRef){
	XScuGic_Disable(&GicInst, IRQ_F2P0);
	printf("Key mid\n\r");
	switch (state){
		case state_idle:
			break;
		case state_change_time:
			en_min();
			break;
		case state_stopwatch:
			sw_stp();
			break;
		case state_timer:
			de_in_min();
			break;
		case state_alarm:
			en_alarm_min();
			break;
		default:
			break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P0);
}

void IRQ_F2P1_Handler(void *CallBackRef){
	XScuGic_Disable(&GicInst, IRQ_F2P1);
	printf("Key down\n\r");
	switch (state){
		case state_idle:
			buzz_off();
			break;
		case state_change_time:
			buzz_off();
			break;
		case state_stopwatch:
			buzz_off();
			break;
		case state_timer:
			buzz_off();
			break;
		case state_alarm:
			buzz_off();
			break;
		default:
			break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P1);
}

void IRQ_F2P2_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P2);
	printf("Key left\n\r");
	switch (state){
		case state_idle:
			break;
		case state_change_time:
			en_hor();
			break;
		case state_stopwatch:
			sw_on();
			break;
		case state_timer:
			str_timer();
			break;
		case state_alarm:
			oled_clear_first_page(&Spi);
			ControlWrite(&Spi, 0x00);
			ControlWrite(&Spi, 0x10);
			ControlWrite(&Spi, 0xb1);
			oled_print(&Spi,"a_on");
			ControlWrite(&Spi, 0x09);
			ControlWrite(&Spi, 0x12);
			ControlWrite(&Spi, 0xb2);
			en_alarm();
			break;
		default:
			break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P2);
}

void IRQ_F2P3_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P3);
	printf("Key right\n\r");
	switch (state){
		case state_idle:
			break;
		case state_change_time:
			en_sec();
			break;
		case state_stopwatch:
			sw_clr();
			break;
		case state_timer:
			cancel_timer();
			break;
		case state_alarm:
			buzz_off();
			oled_clear_first_page(&Spi);
			cancel_alarm();
			ControlWrite(&Spi, 0x09);
			ControlWrite(&Spi, 0x12);
			ControlWrite(&Spi, 0xb2);
			break;
		default:
			break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P3);
}

void IRQ_F2P4_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P4);
	printf("Key up\n\r");
	switch (state){
		case state_idle:
			break;
		case state_change_time:
			break;
		case state_stopwatch:
			sw_clr();
			break;
		case state_timer:
			en_in_min();
			break;
		case state_alarm:
			en_alarm_hor();
			break;
		default:
			break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P4);
}

//----------------------------hold 2 second part
void IRQ_F2P5_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P5);
	printf("Key mid_2\n\r");
	switch (state){
	case state_idle:
		state = state_alarm;
		XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x10);

		oled_clear_second_page(&Spi);
		oled_clear_third_page(&Spi);
		ControlWrite(&Spi, 0x01);
		ControlWrite(&Spi, 0x13);
		ControlWrite(&Spi, 0xb3);
		oled_print(&Spi,"alarm");

		ControlWrite(&Spi, 0x09);
		ControlWrite(&Spi, 0x12);
		ControlWrite(&Spi, 0xb2);
		break;
	default:
		break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P5);
}

void IRQ_F2P6_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P6);
	printf("Key down_2\n\r");
	state = state_idle;
	XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x01);
	oled_clear_second_page(&Spi);
	oled_clear_third_page(&Spi);
	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x13);
	ControlWrite(&Spi, 0xb3);
	oled_print(&Spi,"time");
	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x12);
	ControlWrite(&Spi, 0xb2);
	XScuGic_Enable(&GicInst, IRQ_F2P6);
}

void IRQ_F2P7_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P7);
	printf("Key left_2\n\r");
		switch (state){
		case state_idle:
			state = state_change_time;
			XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x02);
			oled_clear_third_page(&Spi);
			ControlWrite(&Spi, 0x09);
			ControlWrite(&Spi, 0x11);
			ControlWrite(&Spi, 0xb3);
			oled_print(&Spi,"change time");

			ControlWrite(&Spi, 0x01);
			ControlWrite(&Spi, 0x12);
			ControlWrite(&Spi, 0xb2);
			break;
		default:
			break;
		}
	XScuGic_Enable(&GicInst, IRQ_F2P7);
}

void IRQ_F2P8_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P8);
	printf("Key right_2\n\r");
	switch (state){
	case state_idle:
		state = state_timer;
		XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x08);
		oled_clear_third_page(&Spi);
		oled_clear_second_page(&Spi);

		ControlWrite(&Spi, 0x01);
		ControlWrite(&Spi, 0x13);
		ControlWrite(&Spi, 0xb3);

		oled_print(&Spi,"timer");
		ControlWrite(&Spi, 0x09);
		ControlWrite(&Spi, 0x12);
		ControlWrite(&Spi, 0xb2);

		break;
	default:
		break;
	}
	XScuGic_Enable(&GicInst, IRQ_F2P8);
}

void IRQ_F2P9_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P9);
	printf("Key up_2\n\r");
	switch (state){
		case state_idle:
			state = state_stopwatch;
			XGpio_WriteReg(XPAR_AXI_GPIO_2_BASEADDR, 0, 0x04);

			oled_clear_third_page(&Spi);

			ControlWrite(&Spi, 0x09);
			ControlWrite(&Spi, 0x11);
			ControlWrite(&Spi, 0xb3);
			oled_print(&Spi, "stop watch");

			ControlWrite(&Spi, 0x01);
			ControlWrite(&Spi, 0x12);
			ControlWrite(&Spi, 0xb2);
			break;
		default:
			break;
			}
	XScuGic_Enable(&GicInst, IRQ_F2P9);
}


void IRQ_F2P10_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P9);
	printf("timer interrput\n\r");
	buzz_on();
	cancel_timer();

	XScuGic_Enable(&GicInst, IRQ_F2P9);
}

void IRQ_F2P11_Handler(){
	XScuGic_Disable(&GicInst, IRQ_F2P9);
	printf("alarm interrput\n\r");
	buzz_on();
	cancel_alarm();
	XScuGic_Enable(&GicInst, IRQ_F2P9);
}

//----------------------------------------------------
// This function connects the interrupt handler of the interrupt controller to
// the processor.
//----------------------------------------------------

int InterruptSystemSetup()
{
	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
								 &GicInst);
	//Enable interrupts in the ARM
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}


int SetupInterrput(){

	int status;
	XScuGic_Config *GicCfg;
	//Initialize the interrupt controller driver
	GicCfg = XScuGic_LookupConfig(XPAR_PS7_SCUGIC_0_DEVICE_ID);
	status = XScuGic_CfgInitialize(&GicInst, GicCfg, GicCfg->CpuBaseAddress);
	if(status != XST_SUCCESS) return XST_FAILURE;
	//Initialize the exception handler
	Xil_ExceptionInit();
	//register the exception handler
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
					 			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
								&GicInst);
	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler performs
	 * the specific interrupt processing for the device - GPIO in this case
	 */
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P0,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P0_Handler,
					  	  	 (void *)0);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P1,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P1_Handler,
					  	  	 (void *)1);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P2,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P2_Handler,
					  	  	 (void *)2);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P3,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P3_Handler,
					  	  	 (void *)3);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P4,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P4_Handler,
					  	  	 (void *)4);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P5,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P5_Handler,
					  	  	 (void *)5);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P6,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P6_Handler,
					  	  	 (void *)6);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P7,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P7_Handler,
					  	  	 (void *)7);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P8,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P8_Handler,
					  	  	 (void *)8);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P9,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P9_Handler,
					  	  	 (void *)9);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P10,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P10_Handler,
					  	  	 (void *)10);
	status = XScuGic_Connect(&GicInst,
							 IRQ_F2P11,
					  	  	 (Xil_ExceptionHandler)IRQ_F2P11_Handler,
					  	  	 (void *)11);


	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P0, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P1, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P2, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P3, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P4, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P5, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P6, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P7, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P8, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P9, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P10, 0, INT_TYPE_RISING_EDGE);
	XScuGic_SetPriorityTriggerType(&GicInst, IRQ_F2P11, 0, INT_TYPE_RISING_EDGE);

	XScuGic_Enable(&GicInst, IRQ_F2P0);
	XScuGic_Enable(&GicInst, IRQ_F2P1);
	XScuGic_Enable(&GicInst, IRQ_F2P2);
	XScuGic_Enable(&GicInst, IRQ_F2P3);
	XScuGic_Enable(&GicInst, IRQ_F2P4);
	XScuGic_Enable(&GicInst, IRQ_F2P5);
	XScuGic_Enable(&GicInst, IRQ_F2P6);
	XScuGic_Enable(&GicInst, IRQ_F2P7);
	XScuGic_Enable(&GicInst, IRQ_F2P8);
	XScuGic_Enable(&GicInst, IRQ_F2P9);
	XScuGic_Enable(&GicInst, IRQ_F2P10);
	XScuGic_Enable(&GicInst, IRQ_F2P11);


	return XST_SUCCESS;

}



