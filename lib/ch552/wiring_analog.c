/*
 created by Deqing Sun for use with CH55xduino
 */

// clang-format off
#include "wiring_private.h"
#include "pins_arduino_include.h"
// clang-format on

uint8_t analogRead(__data uint8_t pin)
{
  pin = analogPinToChannel(pin);

  if (pin == NOT_ANALOG)
    return 0;

  ADC_CFG = bADC_EN | bADC_CLK;

  ADC_CTRL = (ADC_CTRL & ~(0x03)) | (0x03 & pin);

  ADC_START = 1;

  while (ADC_START)
    ;

  return ADC_DATA;
}


