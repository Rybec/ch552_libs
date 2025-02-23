void Timer0Interrupt(void) __interrupt(INT_NO_TMR0) __using(1);


void timer0_init(void);

uint32_t micros(void);
uint32_t millis(void);

void delay(__data uint32_t ms);
void delayMicroseconds(__data uint16_t us);

