#include <lpc21xx.h>
#include "uart0.h"
#include <string.h>

char rx_buf[50];      // Store received data
int rx_index = 0;
int msg_ready = 0;    // Flag when full message received

void uart0_init(){
    PINSEL0 |= 0x05;  // TXD0 & RXD0
    U0LCR = 0x83;     // Enable DLAB
    U0DLL = 97;       // Baud rate 9600
    U0LCR = 0x03;     // 8-bit data
    U0IER = 0x01;     // Enable RX interrupt
	VICIntSelect = 0x00000000; // IRQ
  VICVectAddr1 = (unsigned)uart0_ISR;
  VICVectCntl1 = 0x20 | 6; /* UART1 Interrupt */
  VICIntEnable = 1 << 6;   /* Enable UART1 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */
}

void uart0_tx(char ch){
    while(!(U0LSR & 0x20));
    U0THR = ch;
}

void uart0_string(char *s){
    while(*s) uart0_tx(*s++);
}

/* UART interrupt */
void uart0_ISR(void)__irq
{
    char ch;

    if(U0IIR & 0x04)
    {
        ch = U0RBR;

        if(ch == '$')
        {
            rx_buf[rx_index] = '\0';
            msg_ready = 1;
            rx_index = 0;
        }
        else
        {
            if(rx_index == 0)
                memset(rx_buf, 0, sizeof(rx_buf));  // ?? CLEAR

            if(rx_index < 49)
                rx_buf[rx_index++] = ch;
        }
    }

    VICVectAddr = 0;
}
