/*
 * Copyright (c) 2020 by Deqing Sun <ds@thinkcreate.us> (c version for CH552
 * port) Touch key library for arduino CH552.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _WS2812_H_INCLUDED
#define _WS2812_H_INCLUDED

// clang-format off
#include <stdint.h>
// clang-format on


#if !defined(_STRINGIFY)
#define _STRINGIFY(x) #x
#endif
#if !defined(_STR)
#define _STR(x) _STRINGIFY(x)
#endif


#if F_CPU > 24000000L
#error The maximum F_CPU for the CH552 is 24000000
#elif F_CPU == 24000000L
#define WS2812_DELAY 5
#elif F_CPU == 16000000L
#define WS2812_DELAY 2
#elif F_CPU == 12000000L
#define WS2812_DELAY 1
#else
#define WS2812_DELAY 0
#endif


void neopixel_show(__xdata uint8_t addr[], uint32_t len);


#define set_pixel_for_GRB_LED(ADDR, INDEX, R, G, B)                            \
  {                                                                            \
    __xdata uint8_t *ptr = (ADDR) + ((INDEX)*3);                               \
    ptr[0] = (G);                                                              \
    ptr[1] = (R);                                                              \
    ptr[2] = (B);                                                              \
  };
#endif
