/*
 * Copyright (c) 2020 by Deqing Sun <ds@thinkcreate.us> (c version for CH55x
 * port) 2022 by Stephen Erisman <github@serisman.com> (major rewrite) Touch key
 * library for arduino CH55x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _WS2812_DELAY_H_INCLUDED
#define _WS2812_DELAY_H_INCLUDED

#if !defined(_STRINGIFY)
#define _STRINGIFY(x) #x
#endif
#if !defined(_STR)
#define _STR(x) _STRINGIFY(x)
#endif

/*
 * This calculation determines how many DELAY loop iterations are needed to
 * stretch out a bit's cycle period to at least 1.25uS. It will stretch the low
 * time for a '0' bit, and the high time for a '1' bit. Roughly this calculates
 * how many actual clock cycles are in the 1.25uS full bit period (based on
 * F_CPU), and then subtracts the 13 clock cycles that are already spent outside
 * the DELAY loop, and then divides by 4, the number of clock cycles per DELAY
 * loop iteration. It uses larger numbers than would at first seem necessary in
 * order to avoid integer math truncation errors.
 */
#define WS2812_DELAY_CALC ((((F_CPU / 1000L) * 1250L) - 13000000L) / 4)

#if WS2812_DELAY_CALC > 15000000L     // 58.4MHz
#error F_CPU not currently supported
#elif WS2812_DELAY_CALC > 14000000L   // 55.2MHz
#define WS2812_DELAY 15
#elif WS2812_DELAY_CALC > 13000000L   // 52MHz
#define WS2812_DELAY 14
#elif WS2812_DELAY_CALC > 12000000L   // 48.8MHz
#define WS2812_DELAY 13
#elif WS2812_DELAY_CALC > 11000000L   // 45.6MHz
#define WS2812_DELAY 12
#elif WS2812_DELAY_CALC > 10000000L   // 42.4MHz
#define WS2812_DELAY 11
#elif WS2812_DELAY_CALC > 9000000L    // 39.2MHz
#define WS2812_DELAY 10
#elif WS2812_DELAY_CALC > 8000000L    // 36MHz
#define WS2812_DELAY 9
#elif WS2812_DELAY_CALC > 7000000L    // 32.8MHz
#define WS2812_DELAY 8
#elif WS2812_DELAY_CALC > 6000000L    // 29.6MHz
#define WS2812_DELAY 7
#elif WS2812_DELAY_CALC > 5000000L    // 26.4MHz
#define WS2812_DELAY 6
#elif WS2812_DELAY_CALC > 4000000L    // 23.3MHz    24MHz = 4250000
#define WS2812_DELAY 5
#elif WS2812_DELAY_CALC > 3000000L    // 20MHz
#define WS2812_DELAY 4
#elif WS2812_DELAY_CALC > 2000000L    // 16.8MHz
#define WS2812_DELAY 3
#elif WS2812_DELAY_CALC > 1000000L    // 13.6MHz    16MHz = 1750000
#define WS2812_DELAY 2
#elif WS2812_DELAY_CALC > 0           // 10.4MHz
#define WS2812_DELAY 1
#else
#define WS2812_DELAY 0
#endif

#endif
