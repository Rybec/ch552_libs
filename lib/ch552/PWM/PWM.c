#include "ch552/wiring_private.h"
#include "ch552/pins_arduino_include.h"
#include "ch552/wiring_digital.h"

#include <ch552/ch552.h>
#include <ch552/PWM/PWM.h>

void PWM_init(void) {
	PWM_CK_SE = 93;  // DIV by 94 for 1K freq on 24M clk
	PWM_CTRL = 0;
}



// *** At some point rename PWM_write() ***

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(__data uint8_t pin, __xdata uint16_t val) {
  // We need to make sure the PWM output is enabled for those pins
  // that support it, as we turn it off when digitally reading or
  // writing with them.  Also, make sure the pin is in output mode
  // for consistenty with Wiring, which doesn't require a pinMode
  // call for the analog output pins.

  pinMode(pin, OUTPUT);
  if (val == 0) {
    digitalWrite(pin, LOW);
  } else if (val >= 256) {
    digitalWrite(pin, HIGH);
  } else {
    switch (digitalPinToPWM(pin)) {
    case PIN_PWM1:
      PIN_FUNC &= ~(bPWM1_PIN_X);
      PWM_CTRL |= bPWM1_OUT_EN;
      PWM_CTRL |= bPWM_IF_END;
      while ((PWM_CTRL & bPWM_IF_END) == 0)
        ;
      PWM_DATA1 = val;
      break;
    case PIN_PWM2:
      PIN_FUNC &= ~(bPWM2_PIN_X);
      PWM_CTRL |= bPWM2_OUT_EN;
      PWM_CTRL |= bPWM_IF_END;
      while ((PWM_CTRL & bPWM_IF_END) == 0)
        ;
      PWM_DATA2 = val;
      break;
    case PIN_PWM1_:
      PIN_FUNC |= (bPWM1_PIN_X);
      PWM_CTRL |= bPWM1_OUT_EN;
      PWM_CTRL |= bPWM_IF_END;
      while ((PWM_CTRL & bPWM_IF_END) == 0)
        ;
      PWM_DATA1 = val;
      break;
    case PIN_PWM2_:
      PIN_FUNC |= (bPWM2_PIN_X);
      PWM_CTRL |= bPWM2_OUT_EN;
      PWM_CTRL |= bPWM_IF_END;
      while ((PWM_CTRL & bPWM_IF_END) == 0)
        ;
      PWM_DATA2 = val;
      break;
    case NOT_ON_PWM:
    default:
      if (val < 128) {
        digitalWrite(pin, LOW);
      } else {
        digitalWrite(pin, HIGH);
      }
    }
  }
}





void turnOffPWM(__data uint8_t pwm) {
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







