// Functions from wiring_digital.c (normally contained in Arduino.h)


void pinMode(__data uint8_t pin,
             __xdata uint8_t mode); // only P1 & P3 can set mode

static void turnOffPWM(__data uint8_t pwm);

uint8_t digitalRead(__data uint8_t pin);
void digitalWrite(__data uint8_t pin, __xdata uint8_t val);



