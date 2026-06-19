#include <lpc21xx.h>
#include <string.h>
#include "uart1.h"


char buff[100];
int r_flag = 0;

void UART1_isr(void) __irq
{
    static int i = 0;
    char ch;

    if(U1IIR & 0x04)
    {
        ch = U1RBR;

        if(ch == 0x02)   // START
        {
            i = 0;
            memset(buff, 0, sizeof(buff));   // ?? CLEAR BUFFER
        }
        else if(ch == 0x03)  // END
        {
            buff[i] = '\0';
            r_flag = 1;     // READY
        }
        else
        {
            if(i < 99)      // ?? PROTECT
                buff[i++] = ch;
        }
    }

    VICVectAddr = 0;
}
