CFLAGS=

all: clock.rel wiring.rel wiring_digital.rel SPI.rel Servo.rel SoftI2C.rel TouchKey.rel WS2812.rel


clock.rel: clock.c
	sdcc --model-small clock.c -I ../../include -c $(CFLAGS)

wiring.rel: wiring.c
	sdcc --model-small wiring.c -I ../../include -c $(CFLAGS)

wiring_digital.rel: wiring_digital.c
	sdcc --model-small wiring_digital.c -I ../../include -c $(CFLAGS)


PWM.rel:
	$(MAKE) -C PWM CFLAGS="$(CFLAGS)"
	cp PWM/PWM.rel ./
	cp PWM/PWM.lst ./
	cp PWM/PWM.sym ./

SPI.rel:
	$(MAKE) -C SPI CFLAGS="$(CFLAGS)"
	cp SPI/SPI.rel ./
	cp SPI/SPI.lst ./
	cp SPI/SPI.sym ./

Servo.rel:
	$(MAKE) -C Servo CFLAGS="$(CFLAGS)"
	cp Servo/Servo.rel ./
	cp Servo/Servo.lst ./
	cp Servo/Servo.sym ./

SoftI2C.rel:
	$(MAKE) -C SoftI2C CFLAGS="$(CFLAGS)"
	cp SoftI2C/SoftI2C.rel ./
	cp SoftI2C/SoftI2C.lst ./
	cp SoftI2C/SoftI2C.sym ./

TouchKey.rel:
	$(MAKE) -C TouchKey CFLAGS="$(CFLAGS)"
	cp TouchKey/TouchKey.rel ./
	cp TouchKey/TouchKey.lst ./
	cp TouchKey/TouchKey.sym ./

WS2812.rel:
	$(MAKE) -C WS2812 CFLAGS="$(CFLAGS)"
	cp WS2812/WS2812.rel ./
	cp WS2812/WS2812.lst ./
	cp WS2812/WS2812.sym ./

USB.rel:
	$(MAKE) -C USB USB.rel CFLAGS="$(CFLAGS)"
	cp USB/USB.rel ./
	cp USB/USB.lst ./
	cp USB/USB.sym ./

USBhandler.rel:
	$(MAKE) -C USB USBhandler.rel CFLAGS="$(CFLAGS)"
	cp USB/USBhandler.rel ./
	cp USB/USBhandler.lst ./
	cp USB/USBhandler.sym ./

USBconstant.rel:
	$(MAKE) -C USB USBconstant.rel CFLAGS="$(CFLAGS)"
	cp USB/USBconstant.rel ./
	cp USB/USBconstant.lst ./
	cp USB/USBconstant.sym ./

USBCDC.rel:
	$(MAKE) -C USB USBCDC.rel CFLAGS="$(CFLAGS)"
	cp USB/USBCDC.rel ./
	cp USB/USBCDC.lst ./
	cp USB/USBCDC.sym ./





clean:
	-$(MAKE) -C SPI clean
	-$(MAKE) -C Servo clean
	-$(MAKE) -C SoftI2C clean
	-$(MAKE) -C TouchKey clean
	-$(MAKE) -C WS2812 clean
	-$(MAKE) -C USB clean
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
