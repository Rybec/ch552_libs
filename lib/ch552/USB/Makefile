CFLAGS=

USB.rel: USB.c
	sdcc USB.c -I ../../../include -c $(CFLAGS)

USBhandler.rel: USBhandler.c
	sdcc USBhandler.c -I ../../../include -c $(CFLAGS)

USBconstant.rel: USBconstant.c
	sdcc USBconstant.c -I ../../../include -c $(CFLAGS)

USBCDC.rel: USBCDC.c
	sdcc USBCDC.c -I ../../../include -c $(CFLAGS)


clean:
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
