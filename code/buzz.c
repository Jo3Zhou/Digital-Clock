#include "buzz.h"


void buzz_on () {
	BUZZER_mWriteReg(XPAR_BUZZER_0_S00_AXI_BASEADDR, BUZZER_S00_AXI_SLV_REG0_OFFSET, 0x0001);
	printf("buzzer is on\n\r");
}

void buzz_off () {
	BUZZER_mWriteReg(XPAR_BUZZER_0_S00_AXI_BASEADDR, BUZZER_S00_AXI_SLV_REG0_OFFSET, 0x0000);
	printf("buzzer is off\n\r");
}
