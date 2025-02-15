# ch552_libs
Arduino independent ch55xduino libraries, with support for CH552 only

**Currently WIP.**

This includes modules for SPI, Servo motors, SoftI2C, TouchKey, USB, and WS2812 (NeoPixel).

Makefiles are included, which are designed to be used from a project Makefile that copies .rel files into the project or build directory.

This is going to be used as part of a CH552 programming tutorial for assembly and maybe C.  Because of this, support for anything besides the CH552 needs to be removed, to make the library code as easy to read as possible for those using the tutorial.  This necessarily means ripping out support for CH549 and CH559.  Support for those will be left in for the first commit, so that if someone wants to fork this and make a version for one of those, they can do that and roll back to the initial commit.

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
- USB (specifically USB bootloader, device does not present COM port when plugged in, probably requires HardwareSerial, which has not been ported yet)
- TouckKey (the interrupt setup for this is currently in arduino_init.c and needs to be moved to the TouchKey module)


## Current Incomplete Work
- [X] Test USB bootloader (Failed, device does not present COM port when plugged in.)
- [ ] Test new PWM module
- [ ] Test other modules that haven't been tested yet to whatever degree I can.  TouchKey probably does not work, due to missing interrupt code (currently in arudino_init.c).  I can move that to TouchKey and then test.  I don't have C/assembly drivers for the SPI hardware I have (well, I do, for the MSP430, but porting will definitely be pain), so I can't test SPI yet.  I can probably test Servo, as I do have some servos.  Some adjustment might be necessary, as it is probably dependent on the PWM code I moved into its own module (this could be a good test of the PWM module).  I don't know enough about USB to test that module myself.  If USB bootloader triggering works though, that would be a good start.  I have prior experience with porting Arduino SoftI2C to be used outside of Arduino with the ESP32, and it was not pleasant.  I do have some devices I can test it with though, and writing drivers for things like temperature/humidity sensors shouldn't be difficult.
- [ ] Create optional (hopefully, not sure of dependencies that may exist in other modules) timer module with all of the timer related code.
- [ ] wiring has been used as dumping grounds for miscellaneous code, including clock rate dependent code, timer0 setup and management, timing delay code, and intitialization code.  This stuff should be handled separately, with any order dependencies documented.
- [ ] wiring_digital needs some refactoring/reorganization.  It currently contains pin configuration code and pin read and write code.
- [ ] wiring_analog only contains analogRead() now, because analogWrite() is not true analog and was moved to the PWM module.  Let's turn wiring_analog into an ADC module, since that's all that's left.
- [ ] arduino_init.c needs to be eliminated.  It contains code that some of the modules are dependent on.  Anything in it that are dependencies need to be moved to the dependent module, if there is only one, or moved into their own new module if multiple modules are dependent.  Note that arduino_init.c is not currently included in the Makefile tree, so it's not actually being used.  It mostly contains interrupt setup code, including for touchkey, serial communication, timer, USB, and GPIO.  It also contains a barebones main() that demonstrates how to keep USB happy when the USB bootloader is active.
- [ ] Add some missing elements from ch55xduino, ideally as separate modules.  This includes wiring_analog, HardwareSerial, Serial, Print (maybe), Print-float (maybe), WInterrupts, WMath, and maybe a few others.  The specific files in question are in ch55xduino/ch55xduino/ch55x/cores/ch55xduino, within the ch55xdiuno package.  There are also a couple of subdirectories that probably need to be looked at.
- [ ] Maybe make assembly delays used in clock.c into a function.  How expensive will it be to calculate cycles to achieve required delay?  Might be possible to make more predictable, by even aligning the function...


## Finished Work
- wiring dependency on timer code eliminated (10ms and 5ms calls to timer delay function replaced with assembly loops)
- Removed from wiring: PWM setup
- Removed from wiring_digital: PWM disabling code
- Removed from wiring_analog: PWM code
- New PWM module created
- Remove all code that doesn't apply to the CH552.  Many of the files use ifdefs to select between code for the CH551, CH552, CH549, and CH559, as well as between various clock speeds.  Any of these that are not for the CH552 need to be eliminated.  This includes clock speeds higher than 24MHz and clock speed dependent defines for clock speeds over 24MHz.
- Remove superfluous stuff, for example, doc/ contains a lot of SDDC documentation as well as other stuff that probably shouldn't be included (maybe doc/ itself can be removed, but first need to check what is there and see if there is anything that should be kept)


## Future Plans
- Add modules for some LCD displays
- Add modules for PWM, Serial, and other on-chip peripherals (Some of these already exist and just need to be broken out of the remaining Arduino code and turned into their own modules.)
- (Moved from incomplete work.  This is going to be quite difficult, if it is even possible, so it is not going to be a priority any time soon.) See if we can find a better way to deal with WS2812.  Currently it contains a .c file for every pin that NeoPixels could be connected to, the mapping is set at compile time using defines, and using multiple pins for NeoPixels is messy.  This appears to be because the NeoPixel code is mainly writen in assembly, and dynamically choosing pins at the assembly level is hard.  It should be possible though!  (In GAS it is possible to pull arguments into in-line assembly, so maybe the same mechanic can be used in SDCC.  Alternatively, I could just rewrite the WS2812 code as a pure assembly function, that can take C arguments and selects the pin based on that.  The pins _are_ memory mapped, maybe that can be leveraged to make argument based selection simple.)
- (Moved from incomplete work.  A _lot_ of timer functionality uses this at the assembly level.  A complete rewrite is needed, maybe with this as one option, but the assembly math for converting from timer overflows to microseconds and milliseconds is going to take me some dedicated time and effort to understand and adapt.  That or I'll just setup the timer in a way to make it much simpler.  Either way, this is not critical right now.  I'll just turn this into an optional module (as much as possible...) and users can forego it and setup their own timer code if they need to.) The timer0 code is currently using r0 through r4 in register bank 12.  SDCC is prevented from using these registers for something else with a pair of global variables set to the addressse of r0-r3, and r4, of register bank 1.  While this is a slick way of optimizing timer performance (interactions with registers are cheaper than interactions with memory addresses), it does not seem ideal for assembly language programming.  Maybe move this to register bank 3, so that the first three can be used contiguously.  Also, it probably wouldn't hurt to use the last 5 instead of the first 5, so that bank 3 can just be treated as having 3 registers starting from r0.  Lastly, why 5?  That's ~72 quadrillion ticks worth before it rolls over (16 bit timers plus 40 bits in registers = 2**56 ~= 72e15).  At 24MHz, that's ~95 years.  (At 16MHz, it's about 142 years.)  That seems _incredibly_ excessive for most applications.  Maybe we need to make a more complete timer module with selectable resolution.  4 register bytes is 135 days @ 24MHz, or 203 days @ 16MHz.  For applications where timing past 4 months (6 months for 16MHz) is important, 5 registers makes sense.  For most applications, 2 to 4 bytes is probably plenty.  Beyond 4 bytes, error accumulation may be too high to be useful (maybe test?, and if this is true, make 4 registers the maximum).  Let's also do an option allowing the user to define the interrupt handler, without having to do all of the initialization work manually.
