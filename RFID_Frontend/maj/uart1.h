#ifndef UART1_H
#define UART1_H

extern int r_flag;
extern char buff[100];

void InitUART1(void);
void UART1_isr(void) __irq;

#endif
