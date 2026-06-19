#include <lpc214x.h>
#include "extint.h"
#include "state.h"

int add_flag=0, del_flag=0, pay_flag=0;

void ext_init(){
    PINSEL1 |= 1<<0;
    PINSEL0 |= 1<<29;
    PINSEL0 |= 1<<31;

    EXTMODE=7;
    EXTPOLAR=0;

    VICVectAddr2=(unsigned)eint0_ISR;
    VICVectCntl2=0x20|14;

    VICVectAddr3=(unsigned)eint1_ISR;
    VICVectCntl3=0x20|15;

    VICVectAddr4=(unsigned)eint2_ISR;
    VICVectCntl4=0x20|16;

    VICIntEnable|=(1<<14)|(1<<15)|(1<<16);
}

void eint0_ISR(void)__irq
{
 state = STATE_ADD;
 EXTINT = 1<<0;
 VICVectAddr = 0;
}

void eint1_ISR(void)__irq
{
 state = STATE_DELETE;
 EXTINT = 1<<1;
 VICVectAddr = 0;
}

void eint2_ISR(void)__irq
{
 state = STATE_PAYMENT;
 EXTINT = 1<<2;
 VICVectAddr = 0;
}
