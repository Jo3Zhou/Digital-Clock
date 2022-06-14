#ifndef OLED_H
#define OLED_H

#include "xparameters.h"
#include "xgpio.h"
#include "xspi.h"
#include "xstatus.h"
#include "xspi_l.h"

#define LED_DELAY	  1000000

//These are the instances of two GPIOs and Quad SPI
XGpio Gpio0; // D/C# The driver instance for GPIO Device configured as O/P
XGpio Gpio1; // VBAT|VDD|RES The driver instance for GPIO Device configured as O/P
XGpio Gpio2; // led
XSpi Spi; /* The instance of the SPI device */
XSpi_Config *ConfigPtr;	/* Pointer to Configuration data for the Quad SPI */

//Prototype Functions
int ControlWrite(XSpi *InstancePtr, u8 wData);
int DataWrite(XSpi *InstancePtr, u8 wData);

u8 *oled_fontGet(char c);

void oled_sendChar(XSpi *InstancePtr, char c);
void oled_setPageCol(XSpi *InstancePtr, u8 page, u8 col);

void oled_clear(XSpi *InstancePtr);

void oled_print(XSpi *InstancePtr, char *src);

void oled_print_welcome();

int oled_initialize();

void oled_off();

void oled_clear_first_page(XSpi *InstancePtr);
void oled_clear_second_page(XSpi *InstancePtr);
void oled_clear_third_page(XSpi *InstancePtr);


#endif
