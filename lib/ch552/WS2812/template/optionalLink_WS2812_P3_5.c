// This file is generated by a script.

/*
 * Copyright (c) 2020 by Deqing Sun <ds@thinkcreate.us> (c version for CH55x
 * port) 2022 by Stephen Erisman <github@serisman.com> (major rewrite) WS2812
 * library for arduino CH55x.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// clang-format off
#include "ch552/WS2812/WS2812.h"
#include "ch552/WS2812/WS2812_DELAY.h"
// clang-format on

// clang-format off
/*
 * F_CPU (MHz):                            56    24    16    12 <=8 >=4     8     6     4
 *                                      ----- ----- ----- ----- ------- ----- ----- -----
 * Clock cycle type (nS):               17.86 41.66  62.5 83.33 125-250   125 166.6   250
 * Timing requirements (# clock cycles):
 *   full bit period (min): (1.25 uS)      70    30    20    15      10    10   7.5     5
 *   bit '0' high (max):    (500 nS)       28    12     8     6       2     4     3     2
 *   bit '1' high (min):    (625 nS)       35    15    10     8       5     5  3.75   2.5
 *   data low (max):        (5 uS)        280   120    80    60      20    40    30    20
 *   reset low (min):       (6 uS, 250 uS for some newer chips)
 * Actual # clock cycles used: (assuming only 4 clock cycles needed for branching)
 *   DELAY (# loop iterations):            15     5     2     1       -
 *   full bit period:    13 + (DELAY*4)    73    33    21    17   13-14
 *   bit '0' high:       4                  4     4     4     4       2
 *   bit '0' low:        9 + (DELAY*4)     69    29    17    13      11
 *   bit '1' high:       6 + (DELAY*4)     66    26    14    10     4-5
 *   bit '1' low:        7                  7     7     7     7       9
 * Sources:
 *   https://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/
 *   https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
 */
// clang-format on

// clang-format off
void neopixel_show_long_P3_5(uint32_t dataAndLen) {
  //'dpl' (LSB),'dph','b' & 'acc'
  //DPTR is the array address, B is the low byte of length
#if F_CPU < 4000000L
  #error WS2812 Requires at least 4 MHz clock
#else
  __asm__("    mov r3, b                           \n"

          ";save EA to R6                          \n"
          "    mov c,_EA                           \n"
          "    clr a                               \n"
          "    rlc a                               \n"
          "    mov r6, a                           \n"
          ";disable interrupt                      \n"
          "    clr _EA                             \n"

          "byteLoop$:                              \n" // [bytes, cycles]
          "    movx  a,@dptr                       \n" // [1,1]
          "    inc dptr                            \n" // [1,1]
          "    mov r2,#8                           \n" // [2,2]
          "bitLoop$:                               \n"
          "    rlc a                               \n" // [1,1]
#if F_CPU > 8000000L
          "    setb _P3_5                          \n" // [2,2]
          "    jc bit7High$                        \n" // [2,2/4|5]
          "    clr _P3_5                           \n" // [2,2]
          "bit7High$:                              \n"
  #if WS2812_DELAY > 0
          "    mov r1,#"_STR(WS2812_DELAY)"        \n" // [2,2] delay 4 clock cycles per iteration
          "bitDelay$:                              \n"
          "    djnz r1,bitDelay$                   \n" // [2,2/4|5|6]
  #endif
#else // F_CPU between 4 MHz and 8 MHz
          "    jnc bit7Low$                        \n" // [2,2/4|5]
          "    setb _P3_5                          \n" // [2,2]
  #if F_CPU > 6000000L
          "    nop                                 \n" // [1,1]
  #endif
          "bit7Low$:                               \n"
          "    setb _P3_5                          \n" // [2,2]
#endif
          "    clr _P3_5                           \n" // [2,2]
          "    djnz r2,bitLoop$                    \n" // [2,2/4|5|6]
          "    djnz r3,byteLoop$                   \n" // [2,2/4|5|6]

          ";restore EA from R6                     \n"
          "    mov a,r6                            \n"
          "    jz  skipRestoreEA_NP$               \n"
          "    setb  _EA                           \n"
          "skipRestoreEA_NP$:                      \n"
          );
          (void)dataAndLen;
#endif
}
// clang-format on
