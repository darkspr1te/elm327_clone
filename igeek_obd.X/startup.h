#define interrupts_off ININTCON &= 0b01111111
#define interrupts_on ININTCON  |= 0b100000001
#define interrupts
#define code

void high_isr(void);
void low_isr(void);
void startUp_interrupts(void);
void startUp_timer(void);
void startUP_GPIO(void);
void startUP_OSCILLATOR(void);
void startUp_device(void);
