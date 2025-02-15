CFLAGS=

USB.rel: USB.c
	sdcc --model-small USB.c -I ../../../include -c $(CFLAGS)

USBhandler.rel: USBhandler.c
	sdcc --model-small USBhandler.c -I ../../../include -c $(CFLAGS)

USBconstant.rel: USBconstant.c
	sdcc --model-small USBconstant.c -I ../../../include -c $(CFLAGS)

USBCDC.rel: USBCDC.c
	sdcc --model-small USBCDC.c -I ../../../include -c $(CFLAGS)


clean:
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
