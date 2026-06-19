#include <lpc214x.h>
#include "state.h"
#include "menu.h"
#include "cart.h"
#include "uart0.h"
#include "uart1.h"
#include "rfid.h"
#include "extint.h"
#include "LCD.h"
#include "kpm.h"
#include "delay.h"

State state = STATE_IDLE;

int main()
{
	InitKPM();
    Init_LCD();
    uart0_init();
    InitUART1();
    ext_init();

    CmdLCD(0x01);
    StrLCD("SMART CART");
    delay_ms(1000);

    while(1)
    {
        switch(state)
        {
            case STATE_IDLE:
                show_menu();
                break;

            case STATE_ADD:
                add_item();
                state = STATE_IDLE;
                break;

            case STATE_DELETE:
                delete_item();
                state = STATE_IDLE;
                break;

            case STATE_PAYMENT:
                payment();
                state = STATE_IDLE;
                break;

           /* case STATE_ADMIN:
                admin_menu();
                state = STATE_IDLE;
                break;	   */
        }
    }
}
