#include <lpc214x.h>
#include "uart0.h"

char rx_buf[50];
int rx_index=0, msg_ready=0;

void uart0_init(){
    PINSEL0 |= 0x05;
    U0LCR=0x83;
    U0DLL=97;
    U0LCR=0x03;
    U0IER=0x01;
}

void uart0_tx(char ch){
    while(!(U0LSR&0x20));
    U0THR=ch;
}

void uart0_string(char *s){
    while(*s) uart0_tx(*s++);
}

void uart0_ISR(void)__irq{
    char ch=U0RBR;
    if(ch=='$'){
        rx_buf[rx_index]='\0';
        msg_ready=1;
        rx_index=0;
    }else{
        rx_buf[rx_index++]=ch;
    }
    VICVectAddr=0;
}