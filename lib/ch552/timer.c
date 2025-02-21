/*
 created by Deqing Sun for use with CH55xduino
 */

#include "ch552/wiring_private.h"

#include "ch552/ch552.h"  // Needed for F_CPU


// These were originally in ch55xduino/ch55x/cores/ch55duino/main.c
// Given that they are only ever used here in wiring.c, and we don't
// want to pull in main.c from Arduino, they are here replacing the
// 'extern' references to them.  (Not sure why they weren't put here
// in the first place, given that they seem to reference specific
// memory locations, and that means declaring them this way in
// multiple places wouldn't be any more of a problem than externing
// them into multiple places.)
__idata __at (0x08) volatile uint32_t timer0_overflow_count = 0;
__idata __at (0x0C) volatile uint8_t timer0_overflow_count_5th_byte = 0;

#define T0_CYCLE 250


// using register bank 1
void Timer0Interrupt(void) __interrupt(INT_NO_TMR0) __using(1) {
  /*timer0_overflow_count++;
   */ //when putting timer0_millis and timer0_overflow_count in bank 1, C code is no longer correct
  __asm__(";Increase timer0_overflow_count on R0~R4(5bytes)\n"
          "    inc r0                                      \n"
          "    cjne r0,#0,incTimer0_overflow_countOver$    \n"
          "    inc r1                                      \n"
          "    cjne r1,#0,incTimer0_overflow_countOver$    \n"
          "    inc r2                                      \n"
          "    cjne r2,#0,incTimer0_overflow_countOver$    \n"
          "    inc r3                                      \n"
          "    cjne r3,#0,incTimer0_overflow_countOver$    \n"
          "    inc r4                                      \n"
          "incTimer0_overflow_countOver$:                  \n");
}

#pragma save
#pragma disable_warning 59
uint32_t micros(void) {
  /*uint32_t m;
   uint8_t t;
   uint8_t interruptOn = EA;
   EA = 0;

   m = timer0_overflow_count;
   t = TL0;

   if ((TF0) && (t < 255)){
   m++;
   }

   if (interruptOn) EA = 1;*/

  __asm__(";uint8_t interruptOn = EA; //to c            \n"
          ";clr and mov won't affect c                  \n"
          "    mov c,_EA                                \n"
          ";EA = 0;                                     \n"
          "    clr _EA                                  \n"
          ";Copy _timer0_overflow_count to local R0~R3,m\n"
          "    mov r0, (_timer0_overflow_count)         \n"
          "    mov r1, (_timer0_overflow_count)+1       \n"
          "    mov r2, (_timer0_overflow_count)+2       \n"
          "    mov r3, (_timer0_overflow_count)+3       \n"
          ";Copy TL0 to local R4, t                     \n"
          "    mov r4, _TL0                             \n"
          ";Copy TCON (TF0) to local R5                 \n"
          "    mov b, _TCON                             \n"
          ";if (interruptOn) EA = 1;                    \n"
          "    mov _EA,c                                \n"

          ";if ((TF0 in b) && (R4 != 255)){             \n"
          "    jnb b.5,incTimer0_overf_cntCopyOver$     \n"
          "    mov a,#1     \n"
          "    add a,r4     \n"
          "    jz incTimer0_overf_cntCopyOver$\n"

          ";m++                                         \n"
          "    inc r0                                   \n"
          "    cjne r0,#0,incTimer0_overf_cntCopyOver$  \n"
          "    inc r1                                   \n"
          "    cjne r1,#0,incTimer0_overf_cntCopyOver$  \n"
          "    inc r2                                   \n"
          "    cjne r2,#0,incTimer0_overf_cntCopyOver$  \n"
          "    inc r3                                   \n"
          "incTimer0_overf_cntCopyOver$:                \n");

  // since TL0 (R4) always ranging from (256-T0_CYCLE) to 255, we can reduce R4
  // by (256-T0_CYCLE)

  __asm__("    clr c                                    \n"
          "    mov a, r4                                \n"
          "    subb a,#6                                \n"
          "    mov r4, a                                \n");

#if F_CPU == 16000000
  // 1m = 250t 1t=0.75us (m*250+t)*3/2/2
  // m in r0~r3
  // t in r4
  // t=((3*t)>>1);
  // m=(m*375)=m<<8+m*119;
  // return (m+t)>>1

  __asm__(";1m = 250t 1t=0.5us (m*250+t)*3/2/2  t is 0~249\n"
          ";we need to return (m<<8+m*119+((3*t)>>1)))>>1 \n"

          ";m=m*119;                                    \n"
          "    mov a, r0                                \n"
          "    mov r6, a                                \n"
          "    mov a, r1                                \n"
          "    mov r7, a                                \n"
          "    push ar3                                 \n"
          "    push ar2                                 \n"

          "    mov b, #119                              \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, a                                \n"
          "    mov r5, b                                \n"

          "    mov b, #119                              \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r1, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #119                              \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r2, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #119                              \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r3, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          ";m=m+m<<8;                                   \n"

          "    mov a, r6                                \n"
          "    add a, r1                                \n"
          "    mov r1, a                                \n"
          "    mov a, r7                                \n"
          "    addc a, r2                               \n"
          "    mov r2, a                                \n"
          "    pop a                                    \n"
          "    addc a, r3                               \n"
          "    mov r3, a                                \n"
          "    pop a                                    \n"
          "    addc a, r5                               \n"
          "    mov r5, a                                \n"

          ";t=((3*t)>>1));                              \n"

          "    mov b, #3                                \n"
          "    mov a, r4                                \n"
          "    mul ab                                   \n"
          "    mov r4, a                                \n"
          "    mov a, b                                 \n"
          "    clr c                                    \n"
          "    rrc a                                    \n"
          "    mov r6, a                                \n"
          "    mov a, r4                                \n"
          "    rrc a                                    \n"
          "    mov r4, a                                \n"

          ";get m+t                                     \n"
          "    mov r7, #0                               \n"
          "    mov a, r4                                \n"
          "    add a, r0                                \n"
          "    mov r0, a                                \n"
          "    mov a, r6                                \n"
          "    addc a, r1                               \n"
          "    mov r1, a                                \n"
          "    mov a, r7                                \n"
          "    addc a, r2                               \n"
          "    mov r2, a                                \n"
          "    mov a, r7                                \n"
          "    addc a, r3                               \n"
          "    mov r3, a                                \n"
          "    mov a, r7                                \n"
          "    addc a, r5                               \n"
          "    mov r5, a                                \n"

          ";return m+t/2                                \n"
          "    clr c                                    \n"
          "    mov a, r5                                \n"
          "    rrc a                                    \n"
          "    mov r5, a                                \n"
          "    mov a, r3                                \n"
          "    rrc a                                    \n"
          "    mov r3, a                                \n"
          "    mov a, r2                                \n"
          "    rrc a                                    \n"
          "    mov b, a                                 \n"
          "    mov a, r1                                \n"
          "    rrc a                                    \n"
          "    mov dph, a                               \n"
          "    mov a, r0                                \n"
          "    rrc a                                    \n"
          "    mov dpl, a                               \n"
          "    mov a, r3                                \n"

  );
  // ’dpl’ (LSB),’dph’,’b’ & ’acc’
#elif F_CPU == 24000000
  // 24M CLK

  /*1m = 250t 1t=0.5us (m*250+t)/2

  t=(t>>1);
  m=m*125;

  return ( m+t );*/

  // assembly has better support for multiplication

  __asm__(";1m = 250t 1t=0.5us (m*250+t)/2  t is 0~249  \n"
          ";we need to return m*125+t/2                 \n"
          ";t=(t>>1);                                   \n"
          "    mov a,r4                                 \n"
          "    clr c                                    \n"
          "    rrc a                                    \n"
          "    mov r4,a                                 \n"

          ";m=m*125;                                    \n"
          "    mov b, #125                              \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, a                                \n"
          "    mov r5, b                                \n"

          "    mov b, #125                              \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r1, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #125                              \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r2, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #125                              \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r3, a                                \n"

          ";return m+t                                  \n"
          "    mov r5, #0                               \n"
          "    mov a, r4                                \n"
          "    add a, r0                                \n"
          "    mov dpl, a                               \n"
          "    mov a, r1                                \n"
          "    addc a, r5                               \n"
          "    mov dph, a                               \n"
          "    mov a, r2                                \n"
          "    addc a, r5                               \n"
          "    mov b, a                                 \n"
          "    mov a, r3                                \n"
          "    addc a, r5                               \n"

  );
#elif F_CPU == 12000000
  // 12M CLK

  /*1m = 250t 1t=1us (m*250+t)

   t=(t);
   m=m*250;

   return ( m+t );*/

  // assembly has better support for multiplication

  __asm__(";1m = 250t 1t=1us (m*250+t)    t is 0~249    \n"
          ";we need to return m*250+t                   \n"

          ";m=m*250;                                    \n"
          "    mov b, #250                              \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, a                                \n"
          "    mov r5, b                                \n"

          "    mov b, #250                              \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r1, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #250                              \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r2, a                                \n"
          "    clr a                                    \n"
          "    addc a, b                                \n"
          "    mov r5, a                                \n"

          "    mov b, #250                              \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    add a, r5                                \n"
          "    mov r3, a                                \n"

          ";return m+t                                  \n"
          "    mov r5, #0                               \n"
          "    mov a, r4                                \n"
          "    add a, r0                                \n"
          "    mov dpl, a                               \n"
          "    mov a, r1                                \n"
          "    addc a, r5                               \n"
          "    mov dph, a                               \n"
          "    mov a, r2                                \n"
          "    addc a, r5                               \n"
          "    mov b, a                                 \n"
          "    mov a, r3                                \n"
          "    addc a, r5                               \n"

  );
#else
#error "clock not supported yet"

#endif
  // return values: ’dpl’ 1B, ’dpl’ LSB & ’dph’ 2B,
  // ’dpl’, ’dph’ and ’b’ 3B, ’dpl’,’dph’,’b’ & ’acc’ 4B
}

uint32_t millis(void) {

  // disable interrupts while we read timer0_millis or we might get an
  // inconsistent value (e.g. in the middle of a write to timer0_millis)

  // assembly has better support for 5 byte data type
  // and efficient in shifting

  __asm__(";uint8_t interruptOn = EA; //to c            \n"
          ";clr and mov won't affect c                  \n"
          "    mov c,_EA                                \n"
          ";EA = 0;                                     \n"
          "    clr _EA                                  \n"
          ";Copy _timer0_overflow_count to local R0~R4  \n"
          "    mov r0, (_timer0_overflow_count)         \n"
          "    mov r1, (_timer0_overflow_count)+1       \n"
          "    mov r2, (_timer0_overflow_count)+2       \n"
          "    mov r3, (_timer0_overflow_count)+3       \n"
          "    mov r4, (_timer0_overflow_count)+4       \n"
          ";if (interruptOn) EA = 1;                    \n"
          "    mov _EA,c                                \n");

#if F_CPU == 16000000
  __asm__(";return (timer0_overflow_count*48)>>8        \n"

          "    mov b, #48                               \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, b                                \n"
          ";lowest 8 bit not used (a), r0 free to use   \n"
          "    mov b, #48                               \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          "    mov r5, psw   ;keep c                    \n"
          "    mov dpl, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #48                               \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    mov psw, r5   ;restore c                 \n"
          "    addc a, r0                               \n"
          "    mov r5, psw   ;keep c                    \n"
          "    mov dph, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #48                               \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    mov psw, r5   ;restore c                 \n"
          "    addc a, r0                               \n"
          "    mov r5, psw   ;keep c                    \n"
          "    mov r1, a                                \n"
          "    mov r0, b                                \n"

          "    mov b, #48                               \n"
          "    mov a, r4                                \n"
          "    mul ab                                   \n"
          "    mov psw, r5   ;restore c                 \n"
          "    addc a, r0                               \n"

          ";calculation finished, a already in place    \n"
          "    mov b, r1                                \n");

#elif F_CPU == 24000000
  __asm__(";return timer0_overflow_count>>3             \n"
          ";Or: return (timer0_overflow_count<<5)>>8    \n"
          ";Or: return (timer0_overflow_count*32)>>8    \n"
          "    mov b, #32                               \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, b                                \n"
          ";lowest 8 bit not used (a), r0 free to use   \n"
          "    mov b, #32                               \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov dpl, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #32                               \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov dph, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #32                               \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov r1, a                                \n"
          "    mov r0, b                                \n"

          "    mov b, #32                               \n"
          "    mov a, r4                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"

          ";calculation finished, a already in place    \n"
          "    mov b, r1                                \n");
#elif F_CPU == 12000000
  __asm__(";return timer0_overflow_count>>2             \n"
          ";Or: return (timer0_overflow_count<<6)>>8    \n"
          ";Or: return (timer0_overflow_count*64)>>8    \n"
          "    mov b, #64                               \n"
          "    mov a, r0                                \n"
          "    mul ab                                   \n"
          "    mov r0, b                                \n"
          ";lowest 8 bit not used (a), r0 free to use   \n"
          "    mov b, #64                               \n"
          "    mov a, r1                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov dpl, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #64                               \n"
          "    mov a, r2                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov dph, a                               \n"
          "    mov r0, b                                \n"

          "    mov b, #64                               \n"
          "    mov a, r3                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"
          "    mov r1, a                                \n"
          "    mov r0, b                                \n"

          "    mov b, #64                               \n"
          "    mov a, r4                                \n"
          "    mul ab                                   \n"
          "    add a, r0                                \n"
          ";carry won't be set, if I calculated right   \n"

          ";calculation finished, a already in place    \n"
          "    mov b, r1                                \n");
#else
#error "clock not supported yet"

#endif
}
#pragma restore

void delay(__data uint32_t ms) {
  __data uint32_t start = micros();

  while (ms > 0) {
    // yield();
    while (ms > 0 && (micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}

void delayMicroseconds(__data uint16_t us) {
  // call with const, "mov dptr, #CONST" and "lcall", 3 + (6 or 7) cycles,
  // depending on even or odd address call with var, "mov dpl, r", "mov dph, r",
  // and "lcall", 2 + 2 + (6 or 7) cycles, depending on odd or even address The
  // compiler by default uses a caller saves convention for register saving
  // across function calls
  us; // avoid unreferenced function argument warning
#if F_CPU >= 24000000UL
  __asm__(
      ".even                                    \n"
      "    mov  r6, dpl                         \n" // low 8-bit
      "    mov  r7, dph                         \n" // high 8-bit
      "    clr  c                               \n"
      "    mov  a,#0x01                         \n"
      "    subb a, r6                           \n"
      "    clr  a                               \n"
      "    subb a, r7                           \n"
      "    jc skip_0us$                         \n"
      "    ret                                  \n" // return if 0 1 us
                                                    // about 1.2us total
      "    nop                                  \n"
      "skip_0us$:                               \n"
      "    clr  c                               \n" // do some loop init, not
                                                    // useful for 2us but better
                                                    // here
      "    mov  a, #0x02                        \n"
      "    subb a, r6                           \n"
      "    mov  r6, a                           \n"
      "    mov  a, #0x00                        \n"
      "    subb a, r7                           \n"
      "    mov  r7, a                           \n"

      "    nop                                  \n" // keep even
      "    cjne r6,#0x00,loop24m_us$            \n"
      "    cjne r7,#0x00,loop24m_us$            \n"
      "    nop                                  \n"
      "    ret                                  \n" // return if 2us  about 2 us
                                                    // total

      "loop24m_us$:                             \n" // about nus

      "    nop \n nop \n nop \n nop \n nop \n    " // 6+11 cycle
      "    nop \n "
      "loop24m_us_2$:                          \n" // need more test

      "    nop \n nop \n nop \n nop \n nop \n    "
      "    nop \n nop \n nop \n nop \n nop \n    "
      "    nop \n                                "

      "    inc  r6                              \n" // 1 cycle
      "    cjne r6, #0,loop24m_us$              \n" // 6 cycle
      "    inc  r7                              \n" // there will be extra 1
                                                    // cycles for every 256us
      "    cjne r7, #0,loop24m_us_2$            \n"
      "    nop                                  \n");
#elif F_CPU >= 16000000UL
  __asm__(
      ".even                                    \n"
      "    mov  r6, dpl                         \n" // low 8-bit
      "    mov  r7, dph                         \n" // high 8-bit
      "    clr  c                               \n"
      "    mov  a,#0x01                         \n"
      "    subb a, r6                           \n"
      "    clr  a                               \n"
      "    subb a, r7                           \n"
      "    jc skip_0us$                         \n"
      "    ret                                  \n" // return if 0 1 us
                                                    // about 1.7us total
      "    nop                                  \n"
      "skip_0us$:                               \n"
      "    cjne r7,#0x00,skip_2us$              \n"
      "    cjne r6,#0x02,skip_2us$              \n"
      "    ret                                  \n" // return if 2us about 2.3us
                                                    // total
      "    nop                                  \n" // keep even
      "skip_2us$:                               \n"

      "    clr  c                               \n" // do some loop init, not
                                                    // useful for 3us but better
                                                    // here
      "    mov  a, #0x03                        \n"
      "    subb a, r6                           \n"
      "    mov  r6, a                           \n"
      "    mov  a, #0x00                        \n"
      "    subb a, r7                           \n"
      "    mov  r7, a                           \n"

      "    cjne r6,#0x00,loop16m_us$            \n"
      "    cjne r7,#0x00,loop16m_us$            \n"
      "    ret                                  \n" // return if 3us  about 3.4
                                                    // us total

      "loop16m_us$:                             \n" // about n.5us

      "    nop \n nop \n nop \n nop \n nop \n    " // 6+3 cycle
      "    nop \n                                "
      "loop16m_us_2$:                          \n"
      "    nop \n nop \n nop \n                  "

      "    inc  r6                              \n" // 1 cycle
      "    cjne r6, #0,loop16m_us$              \n" // 6 cycle
      "    inc  r7                              \n" // there will be extra 1
                                                    // cycles for every 256us
      "    cjne r7, #0,loop16m_us_2$            \n");
#elif F_CPU >= 12000000UL
  __asm__(
      ".even                                    \n"
      "    mov  r6, dpl                         \n" // low 8-bit
      "    mov  r7, dph                         \n" // high 8-bit
      "    clr  c                               \n"
      "    mov  a,#0x02                         \n"
      "    subb a, r6                           \n"
      "    clr  a                               \n"
      "    subb a, r7                           \n"
      "    jc skip_0us$                         \n"
      "    ret                                  \n" // return if 0 1 2 us
                                                    // about 2.25us total
      "    nop                                  \n"
      "skip_0us$:                               \n"
      "    clr  c                               \n" // do some loop init, not
                                                    // useful for 3us but better
                                                    // here
      "    mov  a, #0x03                        \n"
      "    subb a, r6                           \n"
      "    mov  r6, a                           \n"
      "    mov  a, #0x00                        \n"
      "    subb a, r7                           \n"
      "    mov  r7, a                           \n"

      "    nop                                  \n" // keep even
      "    cjne r6,#0x00,loop12m_us$            \n"
      "    cjne r7,#0x00,loop12m_us$            \n"
      "    nop                                  \n"
      "    ret                                  \n" // return if 3us  about 2 us
                                                    // total

      "loop12m_us$:                             \n" // about nus

      "    nop \n nop \n nop \n nop \n nop \n    " //
      "    nop \n "
      "loop12m_us_2$:                          \n"  // need more test
      "    inc  r6                              \n" // 1 cycle
      "    cjne r6, #0,loop12m_us$              \n" // 6 cycle
      "    inc  r7                              \n" // there will be extra 1
                                                    // cycles for every 256us
      "    cjne r7, #0,loop12m_us_2$            \n"
      "    nop                                  \n");
#else
#error "clock not supported yet"

#endif

  // return takes 5~6 cycles, depending on even or odd address
}


