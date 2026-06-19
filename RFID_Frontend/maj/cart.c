#include <stdio.h>
#include <string.h>
#include "cart.h"
#include "lcd.h"
#include "uart0.h"
#include "rfid.h"
#include "kpm.h"
#include "state.h"
#include "delay.h"
extern char buff[100];
extern char rx_buf[50];
extern int msg_ready;
extern int r_flag;

#define MAX_ITEMS 50

struct cart {
    char id[20];
    int price;
} items[MAX_ITEMS];

int count = 0;
int total = 0;

/* ---------- UART WAIT ---------- */
/*int wait_response()
{
    while(!msg_ready);

    //if(t <= 0) return 0;

    return msg_ready;
    //return 1;
}*/	   

/* ---------- ADD ---------- */
void add_item()
{
    char card[20];
    int price=0, qty=0;

    CmdLCD(0x01);
    StrLCD("Scan Item");
	r_flag=0;
    while(r_flag==0);
    strcpy(card, buff);
    r_flag = 0;
			  
    // ADMIN CARD
    if(strcmp(card,"12528171")==0)
    {
        state = STATE_ADMIN;
		admin_menu();
        return;
    }

    uart0_tx('C');
    uart0_string(card);
    uart0_tx('$');

   /* if(!wait_response())
    {
        CmdLCD(0x01);
        StrLCD("No Response");
        return;
    }*/
	msg_ready=0;
	while(!msg_ready);

    sscanf(rx_buf,"%d:%d",&price,&qty);

    if(price <= 0)
    {
        CmdLCD(0x01);
        StrLCD("Invalid Item");
        return;
    }

    if(count >= MAX_ITEMS)
    {
        CmdLCD(0x01);
        StrLCD("Cart Full");
        return;
    }

    strcpy(items[count].id, card);
    items[count].price = price;
    count++;
    total += price;

    CmdLCD(0x01);
    StrLCD("Added Rs:");
    U32LCD(price);

    CmdLCD(0xC0);
    StrLCD("Total:");
    U32LCD(total);
	delay_ms(1000);
	CmdLCD(0x01);
}

/* ---------- DELETE ---------- */
void delete_item()
{
    char card[20];
    int i, found=-1;

    if(count == 0)
    {
        CmdLCD(0x01);
        StrLCD("Cart Empty");
        return;
    }

    CmdLCD(0x01);
    StrLCD("Scan Delete");

    while(r_flag==0);
    strcpy(card, buff);
    r_flag = 0;

    for(i=0;i<count;i++)
    {
        if(strcmp(items[i].id,card)==0)
        {
            found=i;
            break;
        }
    }

    if(found==-1)
    {
        CmdLCD(0x01);
        StrLCD("Not Found");
		delay_ms(1000);
        return;
    }

    total -= items[found].price;

    for(i=found;i<count-1;i++)
        items[i]=items[i+1];

    count--;

    CmdLCD(0x01);
    StrLCD("Removed");
	delay_ms(1000);
    CmdLCD(0xC0);
    StrLCD("Total:");
    U32LCD(total);
	
}

/* ---------- PAYMENT ---------- */
void payment()
{
    char k;
    int cash;

    if(count==0)
    {
        CmdLCD(0x01);
        StrLCD("No Items");
        return;
    }

    CmdLCD(0x01);
    StrLCD("1:Cash 2:Card");

    k = KeyScan();
	while(ColScan()==0);
	delay_ms(100);
    CmdLCD(0xc0);
	CharLCD(k);
	delay_ms(1000);
	if(k=='1')
    {
       INPUT1: CmdLCD(0x01);
        StrLCD("Enter Cash");
		U32LCD(total);
		CmdLCD(0xc0);
        cash = ReadNum();

        if(cash >= total)
        {
            CmdLCD(0x01);
            StrLCD("Paid");
			delay_ms(1000);
            count=0;
            total=0;
        }
        else
        {
            CmdLCD(0x01);
            StrLCD("Less Cash");
			delay_ms(1000);
				goto INPUT1;
        }
    }

    else if(k=='2')
    {
        char card[20], buf[50];

        CmdLCD(0x01);
        StrLCD("Scan Card");

        while(!r_flag);
        strcpy(card, buff);
        r_flag=0;

        sprintf(buf,"B%s:%d$",card,total);
        uart0_string(buf);
		msg_ready=0;
	while(!msg_ready);
        //if(!wait_response()) return;

        if(strcmp(rx_buf,"ENTERPIN")==0)
        {
            int pin, attempts=0;

            while(attempts<3)
            {
                CmdLCD(0x01);
                StrLCD("Enter PIN");

                pin = ReadNum();

                sprintf(buf,"P%d$",pin);
                uart0_string(buf);
				msg_ready=0;
				while(!msg_ready);
                //if(!wait_response()) return;

                if(strcmp(rx_buf,"SUCCESS")==0)
                {
                    CmdLCD(0x01);
                    StrLCD("Success");
					delay_ms(1000);
                    count=0;
                    total=0;
                    return;
                }
                else
                {
                    CmdLCD(0x01);
                    StrLCD("Wrong PIN");
					delay_ms(1000);
                    attempts++;
                }
            }

            CmdLCD(0x01);
            StrLCD("Blocked");
			delay_ms(1000);
        }
    }
}

/* ---------- ADMIN ---------- */
void admin_menu()
{
    char k,id[20],buf[50];
    int price;

    CmdLCD(0x01);
    StrLCD("1:AddStock");

    k = KeyScan();

    if(k=='1')
    {
        CmdLCD(0x01);
        StrLCD("Scan Item");

        while(!r_flag);
        strcpy(id,buff);
        r_flag=0;

        CmdLCD(0x01);
        StrLCD("Enter Price");

        price = ReadNum();

        sprintf(buf,"A%s:%d$",id,price);
        uart0_string(buf);
		msg_ready=0;
	while(!msg_ready);
	StrLCD("Added OK");
	delay_ms(1000);
	CmdLCD(0X01);
        /*if(wait_response())
        {
            CmdLCD(0x01);
            StrLCD("Added OK");
        } 
        else
        {
            CmdLCD(0x01);
            StrLCD("Fail");
        } */
    }
}
