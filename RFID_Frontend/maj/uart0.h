#ifndef UART0_H

#define UART0_H


void uart0_init(void);

void uart0_tx(char ch);

void uart0_string(char *s);


extern char rx_buf[50];

extern int msg_ready;


void uart0_ISR(void) __irq;


#endif
