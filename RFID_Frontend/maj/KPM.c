#include <lpc21xx.h>
#include "type.h"
#include "defines.h"
#include "kpm_defines.h"
#include "lcd.h"
#include "delay.h"

u8 kpmLUT[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','=','D'}};

/*u8 kpmLUT[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};*/
void InitKPM(void)
{
        WNIBBLE(IODIR1,ROW0,15);
}
u32 ColScan(void)
{
        return((RNIBBLE(IOPIN1,COL0)<15)?0:1);
}

u32 RowCheck(void)
{
        u32 rno;
        for(rno=0; rno<4; rno++)
        {
                WNIBBLE(IOPIN1,ROW0,~(1<<rno));
                if(ColScan()==0)
                {
                        break;
                }
        }
        //make rows as default
        WNIBBLE(IOPIN1,ROW0,0x0);
        return rno;
}

u32 ColCheck(void)
{
        u32 cno;
        for(cno=0; cno<4; cno++)
        {
                if(RBIT(IOPIN1,(COL0+cno))==0)
                {
                        break;
                }
        }
        return cno;
}
u8 KeyScan(void)
{
        u32 rno,cno,keyv;
        //wait for switch press
        while(ColScan());
        //find the row_no
        rno=RowCheck();
        //find the col_no
        cno=ColCheck();
        //get the key value using kpmLUT
        keyv=kpmLUT[rno][cno];
        //wait for switch release
        //while(!ColScan());
        return keyv;
}
u32 ReadNum(void)
{
        u8 key;
        u32 sum=0;
        CmdLCD(0XC0);
            while(1)
        {
        key=KeyScan();
                while(ColScan()==0);
                delay_ms(100);
                if(key>='0'&&key<='9')
                {
                        CharLCD(key);
                        sum=(sum*10)+(key-'0');

                }
                 else if(key == '*')
                        {
                            if(sum > 0)
                        {
                                sum = sum / 10;
                                CmdLCD(0x10);
                                CharLCD(' ');
                                CmdLCD(0x10);
                            }
                        }
                                else if(key=='=')
                        {
                        break;
                        }
        }
        return sum;
}
