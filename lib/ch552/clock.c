/*
 created by Deqing Sun for use with CH55xduino
 */

#include "ch552/wiring_private.h"

#include <ch552/ch552.h>  // Needed for F_CPU

#define T0_CYCLE 250

void init(void) {

  // Assign variables to R0-R2 of register bank 3, to
  // ensure the complier doesn't try to use them for other
  // variables or for stack space.
  #pragma save
  #pragma disable_warning 85
  __idata __at (0x18) volatile uint16_t f_cpu_init_delay_count;
  __idata __at (0x1A) volatile uint8_t f_cpu_init_r1_reload;
  __idata __at (0x1B) volatile uint8_t f_cpu_init_a_store;
  #pragma restore

#if F_EXT_OSC > 0
  // switch to external OSC
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  CLOCK_CFG |= bOSC_EN_XT;


  // Delay at least 10,000uS
  // delayMicroseconds(10000);
  //
  // This is used instead of a timer delay to eliminate coupling between
  // startup and the timer module.
  //
  // There's some room for optimization here and in the 5,000uS delay below.
  // This only counts the 4 cycles used by the inner DJNZ when it jumps (if
  // it is at even alignment).  It does not include the 4 extra cycles (2 for
  // MOV, 4 for the outer DJNZ, minus 2 for the inner DJNZ not jumping) when
  // it does not jump and the inner loop is restarted.  In cases where the
  // outer loop has a significant number of iterations, the delay will be
  // somewhat longer than indicated.  Also, because both DJNZs cannot be evenly
  // aligned at the same time, one will always take 5 or 6 cycles when jumping,
  // potentally increasing the delay by up to 50%.
  //
  // Because these are very brief (10ms and 5ms respectively) start up delays,
  // I'm not terribly concerned that they might be longer than intended.  Their
  // duration should always fall somewhere between what is intended and twice
  // that, and an extra 15ms delay during startup will likely not be noticable.
  //
#if F_CPU == 24000000
  // @24MHz, 1uS ~= 24 cycles
  // 10000uS ~= 240000 cycles
  //  240000 / 4 = 60000 = 240 * 250
  //  240 = 0xF0, 250 = 0xFA
  f_cpu_init_delay_count = 0xFAF0;
  f_cpu_init_r1_reload = 0xFA;
#elif F_CPU == 16000000
  // @16MHz, 1uS ~= 16 cycles
  // 10000uS ~= 160000 cycles
  //  160000 / 4 = 40000 = 200 * 200
  //  200 = 0xC8
  f_cpu_init_delay_count = 0xC8C8;
  f_cpu_init_r1_reload = 0xC8;
#elif F_CPU == 12000000
  // @12MHz, 1uS ~= 12 cycles
  // 10000uS ~= 120000 cycles
  //  120000 / 4 = 30000 = 150 * 200
  //  150 = 0x96, 200 = 0xC8
  f_cpu_init_delay_count = 0xC896;
  f_cpu_init_r1_reload = 0xC8;
#elif F_CPU == 6000000
  // @6MHz, 1uS ~= 6 cycles
  // 10000uS ~= 60000 cycles
  //  60000 / 4 = 15000 = 75 * 200
  //  75 = 0x4B, 200 = 0xC8
  f_cpu_init_delay_count = 0xC84B;
  f_cpu_init_r1_reload = 0xC8;
#elif F_CPU == 3000000
  // @3MHz, 1uS ~= 3 cycles
  // 10000uS ~= 30000 cycles
  //  30000 / 4 = 7500 = 75 * 100
  //  75 = 0x4B, 100 = 0x64
  f_cpu_init_delay_count = 0x644B;
  f_cpu_init_r1_reload = 0x64;
#elif F_CPU == 750000
  // @750KHz, 1uS ~= 0.75 cycles
  // 10000uS ~= 7500 cycles
  //  7500 / 4 = 1875 = 25 * 75
  //  25 = 0x19, 75 = 0x4B
  f_cpu_init_delay_count = 0x4B19;
  f_cpu_init_r1_reload = 0x4B;
#elif F_CPU == 187500
  // @187.5KHz, 1uS ~= 0.1875 cycles
  // 10000uS ~= 1875 cycles
  //  1875 / 4 = 468.75 ~= 7 * 67
  //  7 = 0x07, 67 = 0x43
  f_cpu_init_delay_count = 0x4307;
  f_cpu_init_r1_reload = 0x43;
#else
#warning F_CPU invalid or not set
#endif
                                      // cycles
  __asm__("    mov r3, a        \n"   // 1  Store EA and register bank selection
		  "    clr a            \n"   // 1
		  "    mov c, _EA       \n"   // 2
		  "    rlc a            \n"   // 1
		  "    mov c, _RS1      \n"   // 2
		  "    rlc a            \n"   // 1
		  "    mov c, _RS0      \n"   // 2
		  "    rlc a            \n"   // 1

          "    setb _RS0        \n"   // 2  Set to register bank 3
		  "    setb _RS1        \n"   // 2

		  "R1_loop:             \n"   // Delay more than 240,000 cycles
		  "R0_loop:             \n"
		  "    djnz r0, R0_loop \n"   // 2|4/6  2 when no jump, 6 if on odd address, 4 on even
		  "    mov r0, 0x1A     \n"   // 2      Reset r0 counter
		  "    djnz r1, R1_loop \n"   // 2|4/6

		  "    rrc a            \n"   // 1  Restore EA and register bank selection
		  "    mov _RS0, c      \n"   // 2
		  "    rrc a            \n"   // 1
		  "    mov _RS1, c      \n"   // 2
		  "    rrc a            \n"   // 1
		  "    mov _EA, c       \n"   // 2
		  "    mov a, r3        \n"); // 1


  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  CLOCK_CFG &= ~bOSC_EN_INT;
  SAFE_MOD = 0x00;
#endif

  // set internal clock
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;

#if F_CPU == 24000000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x06; // 24MHz
#elif F_CPU == 16000000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x05; // 16MHz
#elif F_CPU == 12000000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x04; // 12MHz
#elif F_CPU == 6000000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x03; // 6MHz
#elif F_CPU == 3000000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x02; // 3MHz
#elif F_CPU == 750000
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x01; // 750KHz
#elif F_CPU == 187500
  CLOCK_CFG = CLOCK_CFG & ~MASK_SYS_CK_SEL | 0x00; // 187.5KHz
#else
#warning F_CPU invalid or not set
#endif

  SAFE_MOD = 0x00;


  // Delay at least 5,000uS
  // delayMicroseconds(5000); // needed to stablize internal RC
#if F_CPU == 24000000
  // @24MHz, 1uS ~= 24 cycles
  // 5000uS ~= 120000 cycles
  //  120000 / 4 = 30000 = 120 * 250
  //  120 = 0x78, 250 = 0xFA
  f_cpu_init_delay_count = 0xFA78;
  f_cpu_init_r1_reload = 0xFA;
#elif F_CPU == 16000000
  // @16MHz, 1uS ~= 16 cycles
  // 5000uS ~= 80000 cycles
  //  80000 / 4 = 20000 = 100 * 200
  //  100 = 0x64, 200 = 0xC8
  f_cpu_init_delay_count = 0xC864;
  f_cpu_init_r1_reload = 0xC8;
#elif F_CPU == 12000000
  // @12MHz, 1uS ~= 12 cycles
  // 5000uS ~= 60000 cycles
  //  60000 / 4 = 15000 = 100 * 150
  //  100 = 0x64, 150 = 0x96
  f_cpu_init_delay_count = 0x9664;
  f_cpu_init_r1_reload = 0x96;
#elif F_CPU == 6000000
  // @6MHz, 1uS ~= 6 cycles
  // 5000uS ~= 30000 cycles
  //  30000 / 4 = 7500 = 75 * 100
  //  75 = 0x4B, 100 = 0x64
  f_cpu_init_delay_count = 0x644B;
  f_cpu_init_r1_reload = 0x64;
#elif F_CPU == 3000000
  // @3MHz, 1uS ~= 3 cycles
  // 5000uS ~= 15000 cycles
  //  15000 / 4 = 3750 = 50 * 75
  //  50 = 0x32, 75 = 0x4B
  f_cpu_init_delay_count = 0x4B32;
  f_cpu_init_r1_reload = 0x4B;
#elif F_CPU == 750000
  // @750KHz, 1uS ~= 0.75 cycles
  // 5000uS ~= 3750 cycles
  //  3750 / 4 = 937.5 ~= 14 * 67
  //  14 = 0x0E, 67 = 0x43
  f_cpu_init_delay_count = 0x430E;
  f_cpu_init_r1_reload = 0x43;
#elif F_CPU == 187500
  // @187.5KHz, 1uS ~= 0.1875 cycles
  // 5000uS ~= 937.5 cycles
  //  937.5 / 4 = 234.375 ~= 1 * 235
  //  1 = 0x01, 235 = 0xEB
  f_cpu_init_delay_count = 0xEB01;
  f_cpu_init_r1_reload = 0xEB;
#else
#warning F_CPU invalid or not set
#endif

  __asm__("    mov r3, a        \n"   // 1  Store EA and register bank selection
		  "    clr a            \n"   // 1
		  "    mov c, _EA       \n"   // 2
		  "    rlc a            \n"   // 1
		  "    mov c, _RS1      \n"   // 2
		  "    rlc a            \n"   // 1
		  "    mov c, _RS0      \n"   // 2
		  "    rlc a            \n"   // 1

          "    setb _RS0        \n"   // 2  Set to register bank 3
		  "    setb _RS1        \n"   // 2

		  "R1_loop:             \n"   // Delay more than 240,000 cycles
		  "R0_loop:             \n"
		  "    djnz r0, R0_loop \n"   // 2|4/6  (Two when no jump, 6 if on odd address, 4 on even)
		  "    mov r0, 0x1A     \n"   // 2
		  "    djnz r1, R1_loop \n"   // 2|4/6

		  "    rrc a            \n"   // 1  Restore EA and register bank selection
		  "    mov _RS0, c      \n"   // 2
		  "    rrc a            \n"   // 1
		  "    mov _RS1, c      \n"   // 2
		  "    rrc a            \n"   // 1
		  "    mov _EA, c       \n"   // 2
		  "    mov a, r3        \n"); // 1



  // init T0 for millis
  TMOD = (TMOD & ~0x0F) | (bT0_M1); // mode 2 for autoreload
  T2MOD = T2MOD & ~bT0_CLK;         // bT0_CLK=0;clk Div by 12
  TH0 = 255 - T0_CYCLE + 1;
  TF0 = 0;
  ET0 = 1;
  TR0 = 1;

  EA = 1; // millis and delay needs interrupt
}
