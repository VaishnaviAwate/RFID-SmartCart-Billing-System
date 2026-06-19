#ifndef RFID_H
#define RFID_H

extern char buff[100];
extern int r_flag;

void UART1_isr(void) __irq;

#endif
