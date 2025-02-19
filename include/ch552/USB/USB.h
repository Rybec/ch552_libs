// Interrupts have to be prototyped in the file containing
// main().  This header _must be_ included for USB Serial
// bootloader triggering to work.
void DeviceUSBInterrupt(void) __interrupt(INT_NO_USB);


void USB_init(void);
