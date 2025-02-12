### CPU Settings ###
# CH552 Board
# CH552 has 16K flash, but 2K is for bootloader, 14K usable
# upload.maximum_size=14336
CODE_SIZE=14336
# QT Py CH552 uses 3.3v, so 16MHz is max
# build.f_cpu=16000000L (16MHz or 12MHz)
BUILD_F_CPU=16000000L
# build.f_oscillator_external=0L
BUILD_F_EXT_OSC=0L
# build.mcu=CH552
BUILD_MCU=CH552
####################

### Bootloader Settings ###
# P3.6 (D+) pull-up
# upload.bootcfg=3
UPLOAD_BOOTCFG=3
###########################

### USB Memory Settings ###
# Default CDC
# upload.maximum_data_size=876
XRAM_SIZE=876
# upload.xdata_location=148
XRAM_LOCATION=148
# build.extra_flags=-DEP0_ADDR=0 -DEP1_ADDR=10 -DEP2_ADDR=20
BUILD_EXTRA_FLAGS=-DEP0_ADDR=0 -DEP1_ADDR=10 -DEP2_ADDR=20

# USER CODE w/ 0B USB ram (Setting this will disable USB upload)
# upload.maximum_data_size=1024
# XRAM_SIZE=1024
# upload.xdata_location=0
# XRAM_LOCATION=0
# build.extra_flags=-DUSER_USB_RAM=0
# BUILD_EXTRA_FLAGS=-DUSER_USB_RAM=0

# USER CODE w/ 148B USB ram (Setting this will disable USB upload)
# upload.maximum_data_size=876
# XRAM_SIZE=876
# upload.xdata_location=148
# XRAM_LOCATION=148
# build.extra_flags=-DUSER_USB_RAM=148
# BUILD_EXTRA_FLAGS=-DUSER_USB_RAM=148

# USER CODE w/ 266B USB ram (Setting this will disable USB upload)
# upload.maximum_data_size=758
# XRAM_SIZE=758
# upload.xdata_location=266
# XRAM_LOCATION=226
# build.extra_flags=-DUSER_USB_RAM=266
# BUILD_EXTRA_FLAGS=-DUSER_USB_RAM=266
###########################

XRAM=--xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOCATION)
MEM_SETTINGS=--code-size $(CODE_SIZE) $(XRAM)

CLOCKS=-DF_CPU=$(BUILD_F_CPU) -DF_EXT_OSC=$(BUILD_F_EXT_OSC)
CPU_SETTINGS=-mmcs51 -D$(BUILD_MCU) $(CLOCKS)


CFLAGS=-DQT_PY_CH552 $(CPU_SETTINGS) $(BUILD_EXTRA_FLAGS) $(MEM_SETTINGS)

INCLUDES=ch552_libs/include

PROJECT_RELS=WS2812.rel wiring_digital.rel wiring.rel USBhandler.rel USBconstant.rel USBCDC.rel


main.ihx: main.c $(PROJECT_RELS)
	sdcc --model-small main.c $(PROJECT_RELS) -I $(INCLUDES) $(CFLAGS)


wiring.rel:
	$(MAKE) wiring.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/wiring.rel ./
	cp ch552_libs/lib/ch552/wiring.lst ./
	cp ch552_libs/lib/ch552/wiring.sym ./

wiring_digital.rel:
	$(MAKE) wiring_digital.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/wiring_digital.rel ./
	cp ch552_libs/lib/ch552/wiring_digital.lst ./
	cp ch552_libs/lib/ch552/wiring_digital.sym ./


WS2812.rel:
	$(MAKE) WS2812.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/WS2812.rel ./
	cp ch552_libs/lib/ch552/WS2812.lst ./
	cp ch552_libs/lib/ch552/WS2812.sym ./


USBhandler.rel:
	$(MAKE) USBhandler.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/USBhandler.rel ./
	cp ch552_libs/lib/ch552/USBhandler.lst ./
	cp ch552_libs/lib/ch552/USBhandler.sym ./


USBconstant.rel:
	$(MAKE) USBconstant.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/USBconstant.rel ./
	cp ch552_libs/lib/ch552/USBconstant.lst ./
	cp ch552_libs/lib/ch552/USBconstant.sym ./

USBCDC.rel:
	$(MAKE) USBCDC.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/USBCDC.rel ./
	cp ch552_libs/lib/ch552/USBCDC.lst ./
	cp ch552_libs/lib/ch552/USBCDC.sym ./


main.hex: main.ihx
	packihx main.ihx > main.hex


upload: main.hex
	ch552_libs/bin/vnproch55x -r 16 -t $(BUILD_MCU) -c $(UPLOAD_BOOTCFG) main.hex

serial_upload: main.hex
	ch552_libs/bin/vnproch55x -s COM6 -t $(BUILD_MCU) -c $(UPLOAD_BOOTCFG) main.hex




clean:
	-$(MAKE) -C ch552_libs/lib/ch552 clean
	-rm *.ihx
	-rm *.lst
	-rm *.sym
	-rm *.rel
	-rm *.asm
	-rm *.rst
	-rm *.lk
	-rm *.map
	-rm *.mem
	-rm *.hex
	-rm *.bin
