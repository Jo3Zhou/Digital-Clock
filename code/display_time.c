#include <stdio.h>
#include "display_time.h"
#include "stop_watch.h"

u8 get_sec(){
	u8 sec = TIMER_mReadReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG4_OFFSET);
	return sec;
}

u8 get_min(){
	u8 min = TIMER_mReadReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG5_OFFSET);
	return min;
}

u8 get_hor(){
	u8 hor = TIMER_mReadReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG6_OFFSET);
	return hor;
}

void set_defalut_time(){
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, 0x0009);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, 0x001e);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, 0x001e);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
}

void en_sec(){
	u8 sec = get_sec();
	u8 min = get_min();
	u8 hor = get_hor();
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, hor);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, min);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec);
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
	if (sec < 59){
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec + 1);
	}
	else{
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, 0x0000);
	}
	TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
}

void en_min(){
	u8 min = get_min();
	if (min < 59){
		u8 sec = get_sec();
		u8 hor = get_hor();
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, min + 1);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, hor);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
	}
	else{
		u8 sec = get_sec();
		u8 hor = get_hor();
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, 0x0000);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, hor);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
	}
}


void en_hor(){
	u8 hor = get_hor();
	if (hor < 23){
		u8 sec = get_sec();
		u8 min = get_min();
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, hor + 1);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, min);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
	}
	else{
		u8 sec = get_sec();
		u8 min = get_min();
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG3_OFFSET, 0x0000);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG2_OFFSET, min);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG1_OFFSET, sec);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
		TIMER_mWriteReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
	}
}

void dis_time(){

	char sec;
	char min;
	char hor;
	sec = get_sec();
	min = get_min();
	hor = get_hor();
	char str_sec[32] = {};
	char str_min[32] = {};
	char str_hor[32] = {};
	sprintf(str_sec, "%d", sec);
	sprintf(str_min, "%d", min);
	sprintf(str_hor, "%d", hor);
	char dis_sec[32] = {};
	char dis_min[32] = {};
	char dis_hor[32] = {};
	char dis_time[32] = {};
	char str_zero[] = "0";
	if (sec < 10){
		strcat(dis_sec, str_zero);
		strcat(dis_sec, str_sec);
	}
	else{
	strcat(dis_sec, str_sec);
	}
	if (min < 10){
		strcat(dis_min, str_zero);
		strcat(dis_min, str_min);
	}
	else{
	strcat(dis_min, str_min);
	}
	if (hor < 10){
		strcat(dis_hor, str_zero);
		strcat(dis_hor, str_hor);
	}
	else{
		strcat(dis_hor, str_hor);
	}
	strcat(dis_time, dis_hor);
	strcat(dis_time, " ");
	strcat(dis_time, dis_min);
	strcat(dis_time, " ");
	strcat(dis_time, dis_sec);

	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x12);
	ControlWrite(&Spi, 0xb2);
	oled_print(&Spi, dis_time);
}

void dis_sw_time(){
	char sec;
	char min;
	char hor;
	sec = sw_sec_out();
	min = sw_min_out();
	hor = sw_hor_out();
	char str_sec[32] = {};
	char str_min[32] = {};
	char str_hor[32] = {};
	sprintf(str_sec, "%d", sec);
	sprintf(str_min, "%d", min);
	sprintf(str_hor, "%d", hor);
	char dis_sec[32] = {};
	char dis_min[32] = {};
	char dis_hor[32] = {};
	char str_zero[] = "0";
	char dis_time[32] = {};
	if (sec < 10){
		strcat(dis_sec, str_zero);
		strcat(dis_sec, str_sec);
	}
	else{
	strcat(dis_sec, str_sec);
	}
	if (min < 10){
		strcat(dis_min, str_zero);
		strcat(dis_min, str_min);
	}
	else{
	strcat(dis_min, str_min);
	}
	if (hor < 10){
		strcat(dis_hor, str_zero);
		strcat(dis_hor, str_hor);
	}
	else{
		strcat(dis_hor, str_hor);
	}
	strcat(dis_time, dis_hor);
	strcat(dis_time, " ");
	strcat(dis_time, dis_min);
	strcat(dis_time, " ");
	strcat(dis_time, dis_sec);

	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x12);
	ControlWrite(&Spi, 0xb2);
	oled_print(&Spi, dis_time);

}


u8 get_timer_sec(){
	u8 sec = COUNTER_mReadReg(XPAR_COUNTER_0_S00_AXI_BASEADDR, COUNTER_S00_AXI_SLV_REG2_OFFSET);
	return sec;
}

u8 get_timer_min(){
	u8 min = COUNTER_mReadReg(XPAR_COUNTER_0_S00_AXI_BASEADDR, COUNTER_S00_AXI_SLV_REG3_OFFSET);
	return min;
}


void dis_timer(){
	char sec;
	char min;
	sec = get_timer_sec();
	min = get_timer_min();
	char str_sec[32] = {};
	char str_min[32] = {};
	sprintf(str_sec, "%d", sec);
	sprintf(str_min, "%d", min);
	char dis_sec[32] = {};
	char dis_min[32] = {};
	char str_zero[] = "0";
	char dis_time[32] = {};
	if (sec < 10){
		strcat(dis_sec, str_zero);
		strcat(dis_sec, str_sec);
	}
	else{
	strcat(dis_sec, str_sec);
	}
	if (min < 10){
		strcat(dis_min, str_zero);
		strcat(dis_min, str_min);
	}
	else{
	strcat(dis_min, str_min);
	}

	strcat(dis_time, "00");
	strcat(dis_time, " ");
	strcat(dis_time, dis_min);
	strcat(dis_time, " ");
	strcat(dis_time, dis_sec);

	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x12);
	ControlWrite(&Spi, 0xb2);
	oled_print(&Spi, dis_time);
}

u8 get_alarm_min(){
	u8 min = TIMER_mReadReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG8_OFFSET);
	return min;
}

u8 get_alarm_hor(){
	u8 hor = TIMER_mReadReg(XPAR_TIMER_0_S00_AXI_BASEADDR, TIMER_S00_AXI_SLV_REG9_OFFSET);
	return hor;
}

void dis_alarm(){
	char sec;
	char min;
	sec = get_alarm_min();
	min = get_alarm_hor();
	char str_sec[32] = {};
	char str_min[32] = {};
	sprintf(str_sec, "%d", sec);
	sprintf(str_min, "%d", min);
	char dis_sec[32] = {};
	char dis_min[32] = {};
	char str_zero[] = "0";
	char dis_time[32] = {};
	if (sec < 10){
		strcat(dis_sec, str_zero);
		strcat(dis_sec, str_sec);
	}
	else{
	strcat(dis_sec, str_sec);
	}
	if (min < 10){
		strcat(dis_min, str_zero);
		strcat(dis_min, str_min);
	}
	else{
	strcat(dis_min, str_min);
	}

	strcat(dis_time, dis_min);
	strcat(dis_time, " ");
	strcat(dis_time, dis_sec);
	strcat(dis_time, " ");
	strcat(dis_time, "00");

	ControlWrite(&Spi, 0x01);
	ControlWrite(&Spi, 0x12);
	ControlWrite(&Spi, 0xb2);
	oled_print(&Spi, dis_time);
}

