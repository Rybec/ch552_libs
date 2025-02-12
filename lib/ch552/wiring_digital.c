/*
 created by Deqing Sun for use with CH55xduino
 */


// clang-format off
#include "ch552/wiring_private.h"
#include "ch552/pins_arduino_include.h"

#include "ch552/pins_arduino.h"   //only include once in core
// clang-format on

void pinMode(__data uint8_t pin,
             __xdata uint8_t mode) // only P1 & P3 can set mode
{
  //__data make sure the local varaibles reside in register in large model
  __data uint8_t bit = digitalPinToBitMask(pin);
  __data uint8_t port = digitalPinToPort(pin);

  if (port == NOT_A_PIN)
    return;

  if (mode == INPUT) {
    if (port == P1PORT) {
      P1_MOD_OC &= ~bit;
      P1_DIR_PU &= ~bit;
    } else if (port == P3PORT) {
      P3_MOD_OC &= ~bit;
      P3_DIR_PU &= ~bit;
    }
  } else if (mode == INPUT_PULLUP) {
    if (port == P1PORT) {
      P1_MOD_OC |= bit;
      P1_DIR_PU |= bit;
    } else if (port == P3PORT) {
      P3_MOD_OC |= bit;
      P3_DIR_PU |= bit;
    }
  } else if (mode == OUTPUT) {
    if (port == P1PORT) {
      P1_MOD_OC &= ~bit;
      P1_DIR_PU |= bit;
    } else if (port == P3PORT) {
      P3_MOD_OC &= ~bit;
      P3_DIR_PU |= bit;
    }
  } else if (mode == OUTPUT_OD) {
    if (port == P1PORT) {
      P1_MOD_OC |= bit;
      P1_DIR_PU &= ~bit;
    } else if (port == P3PORT) {
      P3_MOD_OC |= bit;
      P3_DIR_PU &= ~bit;
    }
  }
}

static void turnOffPWM(__data uint8_t pwm) {
  switch (pwm) {
  case PIN_PWM1:
    if ((PIN_FUNC & bPWM1_PIN_X) == 0) {
      PWM_CTRL &= ~bPWM1_OUT_EN;
    }
    break;
  case PIN_PWM2:
    if ((PIN_FUNC & bPWM2_PIN_X) == 0) {
      PWM_CTRL &= ~bPWM2_OUT_EN;
    }
    break;
  case PIN_PWM1_:
    if ((PIN_FUNC & bPWM1_PIN_X) != 0) {
      PWM_CTRL &= ~bPWM1_OUT_EN;
    }
    break;
  case PIN_PWM2_:
    if ((PIN_FUNC & bPWM2_PIN_X) != 0) {
      PWM_CTRL &= ~bPWM2_OUT_EN;
    }
    break;
  }
  pwm;
  return;
}

uint8_t digitalRead(__data uint8_t pin) {
  __data uint8_t pwm = digitalPinToPWM(pin);
  __data uint8_t bit = digitalPinToBitMask(pin);
  __data uint8_t port = digitalPinToPort(pin);

  if (port == NOT_A_PIN)
    return LOW;

  // If the pin that support PWM output, we need to turn it off
  // before getting a digital reading.
  if (pwm != NOT_ON_PWM)
    turnOffPWM(pwm);

  __data uint8_t portBuf = 0;

  switch (port) {
  case P1PORT:
    portBuf = P1;
    break;
  case P2PORT:
    portBuf = P2;
    break;
  case P3PORT:
    portBuf = P3;
    break;
  default:
    break;
  }

  if (portBuf & bit)
    return HIGH;
  return LOW;
}

void digitalWrite(__data uint8_t pin, __xdata uint8_t val) {
  __data uint8_t pwm = digitalPinToPWM(pin);
  __data uint8_t bit = digitalPinToBitMask(pin);
  __data uint8_t port = digitalPinToPort(pin);

  // If the pin that support PWM output, we need to turn it off
  // before doing a digital write.
  if (pwm != NOT_ON_PWM)
    turnOffPWM(pwm);

  // C pointers cannot be used to access the 8051's SFRs (special function
  // registers).

  __data uint8_t interruptOn = EA;
  EA = 0;

  switch (port) {
  case P1PORT:
    if (val == LOW) {
      P1 &= ~bit;
    } else {
      P1 |= bit;
    }
    break;
  case P2PORT:
    if (val == LOW) {
      P2 &= ~bit;
    } else {
      P2 |= bit;
    }
    break;
  case P3PORT:
    if (val == LOW) {
      P3 &= ~bit;
    } else {
      P3 |= bit;
    }
    break;
  default:
    break;
  }

  if (interruptOn)
    EA = 1;
}
