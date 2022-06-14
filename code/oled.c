#include "oled.h"
#include "font.h"

int oled_initialize()
{
	print("---Entering oled_initialize---\n\r");

	u32 Status, Delay; // use Status variable to capture the return value from any initialisations
	u32 ControlReg; //use this variable to store values from a register

	// Wait a small amount of time
	for (Delay = 0; Delay < LED_DELAY; Delay++);

   	print("---Initialising GPIO---\n\r");

	// Initialize the GPIO drivers

	Status = XGpio_Initialize(&Gpio0, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// initialize the led
	Status = XGpio_Initialize(&Gpio2, XPAR_AXI_GPIO_2_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&Gpio2, 1, 0x00);

   	// Set the direction for all signals to be outputs for both GPIOs
	XGpio_SetDataDirection(&Gpio0, 1, 0b0);
	XGpio_SetDataDirection(&Gpio1, 1, 0b000);

	// Initialize the SPI driver
	print("---Initialising SPI---\n\r");

	ConfigPtr = XSpi_LookupConfig(XPAR_AXI_QUAD_SPI_0_DEVICE_ID);
	if (ConfigPtr == NULL){
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(&Spi, ConfigPtr, ConfigPtr -> BaseAddress);
	if (Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	// Setup the control register to enable master mode
	ControlReg = XSpi_GetControlReg(&Spi);
	XSpi_SetControlReg(&Spi, ControlReg | XSP_CR_MASTER_MODE_MASK);

	// Setup the SPISSR
	// The SPI Slave Select Register (SPISSR)
	XSpi_SetSlaveSelectReg(&Spi, 0x0);

	XSpi_IntrGlobalDisable(&Spi);

	//Initialise the OLED

	print("---Initialising OLED---\n\r");
	// Initialisation of the OLED
	// VBAT|VDD|RES this order
	//1. Switch off power to the OLED: VBAT=1; VDD=1; RES=1;
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b111);
	//2. delay
	for (Delay = 0; Delay <LED_DELAY; Delay++);
	//3. Switch on logic: OLED-VDD=0
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b101);
	//4.delay
	for (Delay = 0; Delay <LED_DELAY; Delay++);
	//5. Display off send the command 0xAE and add a Delay
	ControlWrite(&Spi, 0xAE);
	for (Delay = 0; Delay <LED_DELAY; Delay++);
	//6. Generate a reset pulse
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b100);
	for (Delay = 0; Delay <LED_DELAY; Delay++);
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b101);
	//7. Send the commands in the following order (all in hex): 8D, 14, D9, F1
	ControlWrite(&Spi, 0x8D);
	ControlWrite(&Spi, 0x14);
	ControlWrite(&Spi, 0xD9);
	ControlWrite(&Spi, 0xF1);
	//8. Switch on OLED matric: OLED-VBAT=0
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b001);
	//9. Send the commands in the following order (all in hex): 81, 0F, A0, C0, DA, 00, AF
	ControlWrite(&Spi, 0x81);
	ControlWrite(&Spi, 0x0F);
	ControlWrite(&Spi, 0xA0);
	ControlWrite(&Spi, 0xC0);
	ControlWrite(&Spi, 0xDA);
	ControlWrite(&Spi, 0x00);
	ControlWrite(&Spi, 0xAF);

	//clear the screen
   	oled_clear(&Spi);

   	oled_setPageCol(&Spi, 0, 0);

   	return 0;
}

void oled_off(){

	u32 Delay;

	//Power off sequence
	print("---Display off sequence starting---\n\r");

	//1. Display off  send the command 0xAE and add a Delay
	ControlWrite(&Spi, 0xAE);
	//2. Switch off OLED matric: OLED-VBAT=1 and add a Delay
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b101);
	for (Delay = 0; Delay < LED_DELAY; Delay++);
	//3. Switch off logic: OLED-VDD=1 and add a Delay
	XGpio_WriteReg(XPAR_AXI_GPIO_1_BASEADDR, 0, 0b111);
	for (Delay = 0; Delay < LED_DELAY; Delay++);

	print("---Exiting main---\n\r");
}

//write command to oled
int ControlWrite(XSpi *InstancePtr, u8 wData)
{

	u32 StatusReg;
	//u32 Delay;

	//sets the D/C# line to zero and add a delay, and write data to the SPIDTR register
	XGpio_WriteReg(XPAR_GPIO_0_BASEADDR, 0, 0x0);
	//for (Delay = 0; Delay < LED_DELAY; Delay++);

	//write data to the SPIDTR register
	//The SPI Data Transmit Register (SPI DTR) is written with the data to be transmitted on the SPI bus.
	XSpi_WriteReg(InstancePtr->BaseAddr, XSP_DTR_OFFSET, wData);

	//Enable the device and uninhibit master transactions. Preserves the current
	//contents of the control register
	XSpi_Enable(InstancePtr);

	// Wait for the transfer to be done by polling the transmit empty status bit.

	do {
		StatusReg = XSpi_IntrGetStatus(InstancePtr);
	} while ((StatusReg & XSP_INTR_TX_EMPTY_MASK) == 0);

	XSpi_IntrClear(InstancePtr,XSP_INTR_TX_EMPTY_MASK);

	return XST_SUCCESS;
}

//write data to oled
int DataWrite(XSpi *InstancePtr, u8 wData)
{
	u32 StatusReg;
	//u32 Delay;

	//sets the D/C# line to one and add a delay then write data to the SPIDTR register
	XGpio_WriteReg(XPAR_GPIO_0_BASEADDR, 0, 0x1);
	//for (Delay = 0; Delay < LED_DELAY; Delay++);

	//write data to the SPIDTR register
	//The SPI Data Transmit Register (SPI DTR) is written with the data to be transmitted on the SPI bus.
	XSpi_WriteReg(InstancePtr->BaseAddr,XSP_DTR_OFFSET,wData);
	//Enable the device and uninhibit master transactions.
	//Preserves the current contents of the control register
	XSpi_Enable(InstancePtr);
	//Wait for the transfer to be done by polling the transmit
	//empty status bit
	do {
		StatusReg = XSpi_GetStatusReg(InstancePtr);
	} while ((StatusReg & XSP_SR_TX_EMPTY_MASK) == 0);
	XSpi_IntrClear(InstancePtr,XSP_INTR_TX_EMPTY_MASK);
	return XST_SUCCESS;
}

//---------------display char part-------------
u8 *oled_fontGet(char c) {
	return oled_font + (u32)(c<<3);}//c<<3 = c*8

void oled_sendChar(XSpi *InstancePtr, char c) {
	u8 i;
	u8 *pFont = oled_fontGet(c);
	for(i=0;i<8;++i) {
		DataWrite(InstancePtr, pFont[i]);
	}
}

void oled_setPageCol(XSpi *InstancePtr, u8 page, u8 col) {
	ControlWrite(InstancePtr, 0x22); //Set Page Address
	ControlWrite(InstancePtr, page); //A[1:0] start page
	ControlWrite(InstancePtr, 0x03); //B[2:0] end page
	ControlWrite(InstancePtr, 0x21); //Set Column Address
	ControlWrite(InstancePtr, col); //A[6:0] start
	ControlWrite(InstancePtr, 0x7f); //B[6:0] end
}

void oled_clear(XSpi *InstancePtr) {
    u8 page, i;
	for(page=0;page<4;++page) {
		oled_setPageCol(&Spi, page, 0);
		for(i=0;i<16;++i) {
			oled_sendChar(InstancePtr, ' ');
		}
    }
}

void oled_print(XSpi *InstancePtr, char *src) {
	while(*src!=0) {
		oled_sendChar(InstancePtr, *src);
		++src;
	}
}

void oled_clear_first_page(XSpi *InstancePtr) {
    u8 i;
	ControlWrite(&Spi, 0x00);
	ControlWrite(&Spi, 0x10);
	ControlWrite(&Spi, 0xb1);
	for(i=0;i<16;++i) {
		oled_sendChar(InstancePtr, ' ');
	}
}

void oled_clear_second_page(XSpi *InstancePtr) {
    u8 i;
	ControlWrite(&Spi, 0x00);
	ControlWrite(&Spi, 0x10);
	ControlWrite(&Spi, 0xb2);
	for(i=0;i<16;++i) {
		oled_sendChar(InstancePtr, ' ');
	}
}

void oled_clear_third_page(XSpi *InstancePtr) {
    u8 i;
	ControlWrite(&Spi, 0x00);
	ControlWrite(&Spi, 0x10);
	ControlWrite(&Spi, 0xb3);
	for(i=0;i<16;++i) {
		oled_sendChar(InstancePtr, ' ');
	}
}

void oled_print_welcome() {
	ControlWrite(&Spi, 0x22); //Set Page Address
	ControlWrite(&Spi, 0x00); //A[2:0] start page (start at 0)
	ControlWrite(&Spi, 0x03); //B[2:0] end page (end at 3)

	ControlWrite(&Spi, 0x21); //Set Column Address
	ControlWrite(&Spi, 0x00); //A[6:0] start at 0
	ControlWrite(&Spi, 0x7f); //B[6:0] end at 127

	oled_sendChar(&Spi, 'w');
	oled_sendChar(&Spi, 'e');
	oled_sendChar(&Spi, 'l');
	oled_sendChar(&Spi, 'c');
	oled_sendChar(&Spi, 'o');
	oled_sendChar(&Spi, 'm');
	oled_sendChar(&Spi, 'e');
	oled_sendChar(&Spi, ' ');
	oled_sendChar(&Spi, 'Y');
	oled_sendChar(&Spi, '3');
	oled_sendChar(&Spi, '8');
	oled_sendChar(&Spi, '9');
	oled_sendChar(&Spi, '7');
	oled_sendChar(&Spi, '9');
	oled_sendChar(&Spi, '4');
	oled_sendChar(&Spi, '5');
}
