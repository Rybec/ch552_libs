// May require setting up CPU frequency first.
void PWM_init(void);

// This needs renamed to PWM_write(), but any references to
// analogWrite() need changed at the same time.
void analogWrite(__data uint8_t pin, __xdata uint16_t val);

void turnOffPWM(__data uint8_t pwm);
