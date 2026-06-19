#ifndef EXTINT_H
#define EXTINT_H

void ext_init(void);

extern int add_flag, del_flag, pay_flag;

void eint0_ISR(void)__irq;
void eint1_ISR(void)__irq;
void eint2_ISR(void)__irq;

#endif
