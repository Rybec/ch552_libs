
#include "ch552/wiring_private.h"
#include <ch552/ch5xx.h>

#include <ch552/USB/USBhandler.h>


void USB_init(void) {
	USBDeviceCfg();
	USBDeviceEndPointCfg();
	USBDeviceIntCfg();
	UEP0_T_LEN = 0;
	UEP1_T_LEN = 0;
	UEP2_T_LEN = 0;
}


void DeviceUSBInterrupt(void) __interrupt(INT_NO_USB) {
	USBInterrupt();
}

