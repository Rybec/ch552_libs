# ch552_libs
Arduino independent ch55xduino libraries, with support for CH552 only

**Currently WIP.**

This includes modules for SPI, Servo motors, SoftI2C, TouchKey, USB, and WS2812 (NeoPixel).

Makefiles are included, which are designed to be used from a project Makefile that copies .rel files into the project or build directory.

This is going to be used as part of a CH552 programming tutorial for assembly and maybe C.  Because of this, support for anything besides the CH552 needs to be removed, to make the library code as easy to read as possible for those using the tutorial.  This necessarily means ripping out support for CH549 and CH 559.  Support for those will be left in for the first commit, so that if someone wants to fork this and make a version for one of those, they can do that and roll back to the initial commit.

## What Works and What Does Not

At this point (the initial commit), the following have been tested and work:
- WS2812

The following have not been fully tested:
- SPI
- Servo
- SoftI2C
- TouchKey
- USB

The following do not work (either failed testing or there is a known bug):
- USB (specifically USB bootloader, which may require an additional file I have not identified yet)
- TouckKey (the interrupt setup for this is currently in arduino_init.c and needs to be moved to the TouchKey module)


## Current Incomplete Work

- Remove all code that doesn't apply to the CH552.  Many of the files use ifdefs to select between code for the CH551, CH552, CH549, and CH559, as well as between various clock speeds.  Any of these that are not for the CH552 need to be eliminated.  This includes clock speeds higher than 24MHz and clock speed dependent defines for clock speeds over 24MHz.
- wiring has been used as dumping grounds for miscellaneous code, including clock rate dependent code, timer0 setup and management, timing delay code, and intitialization code (including clock speed setup, USB initialiation, PWM clock setup).  This stuff should be handled separately, with any order dependencies documented.
- wiring_digital needs some refactoring/reorganization.  It currently contains pin configuration code, PWM control code, and pin read and write code.  PWM code should probably be broken out and combined with PWM setup code from wiring in a new PWM module.
- arduino_init.c needs to be eliminated.  It contains code that some of the modules are dependent on.  Anything in it that are dependencies need to be moved to the dependent module, if there is only one, or moved into their own new module if multiple modules are dependent.  Note that arduino_init.c is not currently included in the Makefile tree, so it's not actually being used.  It mostly contains interrupt setup code, including for touchkey, serial communication, timer, USB, and GPIO.  It also contains a barebones main() that demonstrates how to keep USB happy when the USB bootloader is active.
- Remove superfluous stuff, for example, doc/ contains a lot of SDDC documentation as well as other stuff that probably shouldn't be included (maybe doc/ itself can be removed, but first need to check what is there and see if there is anything that should be kept)
- See if we can find a better way to deal with WS2812.  Currently it contains a .c file for every pin that NeoPixels could be connected to, the mapping is set at compile time using defines, and using multiple pins for NeoPixels is messy.  This appears to be because the NeoPixel code is mainly writen in assembly, and dynamically choosing pins at the assembly level is hard.  It should be possible though!  (In GAS it is possible to pull arguments into in-line assembly, so maybe the same mechanic can be used in SDCC.  Alternatively, I could just rewrite the WS2812 code as a pure assembly function, that can take C arguments and selects the pin based on that.  The pins _are_ memory mapped, maybe that can be leveraged to make argument based selection simple.)
- The timer0 code is currently using r0 through r4 in register bank 12.  SDCC is prevented from using these registers for something else with a pair of global variables set to the addressse of r0-r3, and r4, of register bank 1.  While this is a slick way of optimizing timer performance (interactions with registers are cheaper than interactions with memory addresses, and the INC instruction can only be used with registers), it does not seem ideal for assembly language programming.  Maybe move this to register bank 3, so that the first three can be used contiguously.  Also, it probably wouldn't hurt to use the last 5 instead of the first 5, so that bank 3 can just be treated as having 3 registers starting from r0.  Lastly, why 5?  That's ~72 quadrillion ticks worth before it rolls over (16 bit timers plus 40 bits in registers = 2**56 ~= 72e15).  At 24MHz, that's ~95 years.  (At 16MHz, it's about 142 years.)  That seems _incredibly_ excessive for most applications.  Maybe we need to make a more complete timer module with selectable resolution.  4 register bytes is 135 days @ 24MHz, or 203 days @ 16MHz.  For applications where timing past 4 months (6 months for 16MHz) is important, 5 registers makes sense.  For most applications, 2 to 4 bytes is probably plenty.  Beyond 4 bytes, error accumulation may be too high to be useful (maybe test?, and if this is true, make 4 registers the maximum).  Let's also do an option allowing the user to define the interrupt handler, without having to do all of the initialization work manually.


## Future Plans

- Add modules for some LCD displays
- Add modules for PWM, Serial, and other on-chip peripherals (Some of these already exist and just need to be broken out of the remaining Arduino code and turned into their own modules.)

