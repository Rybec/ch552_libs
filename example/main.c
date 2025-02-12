#include <stdint.h>  // For bit specific integer types (eg. uint8_t)

#include <ch552/ch5xx.h>          // For OUTPUT macro
#include <ch552/wiring.h>         // For delay(), init()
#include <ch552/wiring_digital.h> // For pinMode()
#include <ch552/WS2812/WS2812.h>  // For NeoPixel functions/macros


#define NEOPIXEL_PIN P1_0
#define NUM_LEDS 1
#define COLOR_PER_LEDS 3
#define NUM_BYTES (NUM_LEDS*COLOR_PER_LEDS)
#if NUM_BYTES > 255
#error "NUM_BYTES can not be larger than 255."
#endif

#define NEOPIXEL_SHOW_FUNC CONCAT(neopixel_show_, NEOPIXEL_PIN)


void rainbowCycle(uint8_t wait);
uint32_t wheel(uint8_t wheelPos);


__xdata uint8_t ledData[3];
uint8_t neopixel_brightness = 100;


void neopixel_begin(void) {
	pinMode(NEOPIXEL_PIN, OUTPUT);
}

void neopixel_show(void) {
	NEOPIXEL_SHOW_FUNC(ledData, NUM_BYTES);
}

void neopixel_setPixelColor(uint8_t i, uint32_t c) {
	uint16_t r, g, b;
	r = (((c >> 16) & 0xFF) * neopixel_brightness) >> 8;
	g = (((c >> 8) & 0xFF) * neopixel_brightness) >> 8;
	b = ((c & 0xFF) * neopixel_brightness) >> 8;

	set_pixel_for_GRB_LED(ledData, i, r, g, b);
}

void neopixel_setBrightness(uint8_t b) {
	neopixel_brightness = b;
}



void main(void) {
	// Initialization, from wiring.c.  Sets clock speed based on F_CPU
	// (set F_CPU in Makefile).  Also initializes clock and and PWM, as
	// well as a few other things (including USB bootloader stuff if
	// USER_USB_RAM is not defined).
	init();  // This can be ommitted, if you handle the above manually.

	// setup
	neopixel_begin();

	//loop
	while(1) {
		rainbowCycle(100);
//		neopixel_setPixelColor(0, 0x440022);
//		neopixel_show();
//		delay(100000);
		USBSerial_flush();  // If USER_USB_RAM is not defined, we need this
//		neopixel_setPixelColor(0, 0x224400);
//		neopixel_show();
//		delay(100000);

	}
}


void rainbowCycle(uint8_t wait) {
	uint8_t i, j;

	for (j = 0; j < 255; j++) {
		for (i = 0; i < NUM_LEDS; i++) {
			neopixel_setPixelColor(i, wheel(((i * 256 / NUM_LEDS) + j) & 255));
		}
		neopixel_show();
		delay(wait);
	}
}


uint32_t wheel(uint8_t wheelPos) {
	uint8_t r, g, b;
	uint32_t c;

	if (wheelPos < 85) {
		r = wheelPos * 3;
		g = 255 - wheelPos * 3;
		b = 0;
	} else if (wheelPos < 170) {
		wheelPos -= 85;
		r = 255 - wheelPos * 3;
		g = 0;
		b = wheelPos * 3;
	} else {
		wheelPos -= 170;
		r = 0;
		g = wheelPos * 3;
		b = 255 - wheelPos * 3;
	}
	c = r;
	c <<= 8;
	c |= g;
	c <<= 8;
	c |= b;
	return c;
}
