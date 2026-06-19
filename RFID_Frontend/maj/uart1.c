#include <lpc21xx.h>
#include "rfid.h"
#define UART_INT_ENABLE 1
void InitUART1 (void) // Initialize Serial Interface        
{  
            		
  PINSEL0 = 0xA0010005 |0xA0050005; // Enable RxD0 and TxD0  
  //PINSEL0 |= (1<<16); // RXD1
  //PINSEL0 |= (1<<18); // TXD1	
  U1LCR = 0x83;         // 8 bits, no Parity, 1 Stop bit     
  U1DLL = 97;           // 9600 Baud Rate @ CCLK/4 VPB Clock
  U1LCR = 0x03;         // DLAB = 0  
  
  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART1_isr;
  VICVectCntl0 = 0x20 | 7; /* UART1 Interrupt */
  VICIntEnable = 1 << 7;   /* Enable UART1 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U1IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   

  #endif
						
}
