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

###########################
# Stack and Data Segment Notes
#
# SDCC puts the data segment and after the highest used register bank by
# default (unless instructed otherwise, SDCC will only use bank 0).  This
# means it can end up using register banks used by in-line asm if you don't
# also declare a C variable at or after that memory address
# (using __at (hex address)).  It will also use the bit addressable RAM space
# for data space, if it is not used or allocated in C code.  This can be
# avoided by specifying where the data space should start, with --data-loc.
# The first address after the register banks is 0x20, and the first address
# after the bit addressable space is 0x30.  Note that this is only necessary
# if you are using in-line assembly or calling assembly code from C code, or
# if you are calling C modules from assembly code.  Otherwise the compiler
# will have enough infomation to avoid conflicts.
#
# SDCC puts the stack directly after the data segment by default.  If there's
# some reason to reserve some space between these segments, --stack-loc can be
# used to specify exactly where the stack should start.  Stack space can be
# put in the upper 128 bytes of internal RAM this way.
############################

XRAM=--xram-size $(XRAM_SIZE) --xram-loc $(XRAM_LOCATION)
MEM_SETTINGS=--code-size $(CODE_SIZE) $(XRAM) --model-small


CLOCKS=-DF_CPU=$(BUILD_F_CPU) -DF_EXT_OSC=$(BUILD_F_EXT_OSC)
CPU_SETTINGS=-mmcs51 -D$(BUILD_MCU) $(CLOCKS)

OPTIMIZERS=

CFLAGS=-DQT_PY_CH552 $(CPU_SETTINGS) $(BUILD_EXTRA_FLAGS) $(MEM_SETTINGS) $(OPTIMIZERS)

INCLUDES=ch552_libs/include

#PROJECT_RELS=clock.rel PWM.rel WS2812.rel wiring_digital.rel wiring.rel HardwareSerial0.rel HardwareSerial0ISR.rel HardwareSerial1.rel HardwareSerial1ISR.rel USB.rel USBhandler.rel USBconstant.rel USBCDC.rel

PROJECT_RELS=clock.rel PWM.rel WS2812.rel wiring_digital.rel wiring.rel USB.rel USBhandler.rel USBconstant.rel USBCDC.rel


main.ihx: main.c $(PROJECT_RELS)
	sdcc main.c $(PROJECT_RELS) -I $(INCLUDES) $(CFLAGS)

clock.rel:
	$(MAKE) clock.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/clock.rel ./
	cp ch552_libs/lib/ch552/clock.lst ./
	cp ch552_libs/lib/ch552/clock.sym ./

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

HardwareSerial0.rel:
	$(MAKE) HardwareSerial0.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/HardwareSerial0.rel ./
	cp ch552_libs/lib/ch552/HardwareSerial0.lst ./
	cp ch552_libs/lib/ch552/HardwareSerial0.sym ./

HardwareSerial0ISR.rel:
	$(MAKE) HardwareSerial0ISR.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/HardwareSerial0ISR.rel ./
	cp ch552_libs/lib/ch552/HardwareSerial0ISR.lst ./
	cp ch552_libs/lib/ch552/HardwareSerial0ISR.sym ./

HardwareSerial1.rel:
	$(MAKE) HardwareSerial1.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/HardwareSerial1.rel ./
	cp ch552_libs/lib/ch552/HardwareSerial1.lst ./
	cp ch552_libs/lib/ch552/HardwareSerial1.sym ./

HardwareSerial1ISR.rel:
	$(MAKE) HardwareSerial1ISR.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/HardwareSerial1ISR.rel ./
	cp ch552_libs/lib/ch552/HardwareSerial1ISR.lst ./
	cp ch552_libs/lib/ch552/HardwareSerial1ISR.sym ./




PWM.rel:
	$(MAKE) PWM.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/PWM.rel ./
	cp ch552_libs/lib/ch552/PWM.lst ./
	cp ch552_libs/lib/ch552/PWM.sym ./

WS2812.rel:
	$(MAKE) WS2812.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/WS2812.rel ./
	cp ch552_libs/lib/ch552/WS2812.lst ./
	cp ch552_libs/lib/ch552/WS2812.sym ./


USB.rel:
	$(MAKE) USB.rel -C ch552_libs/lib/ch552 CFLAGS="$(CFLAGS)"
	cp ch552_libs/lib/ch552/USB.rel ./
	cp ch552_libs/lib/ch552/USB.lst ./
	cp ch552_libs/lib/ch552/USB.sym ./


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
