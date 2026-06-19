#include<lpc21xx.h>
#include "LCD_defines.h"
#include "type.h"
#include "defines.h"
#include "delay.h"
void WriteLCD(u8 data)
{
	 SCLRBIT(IOCLR0,RW);//RW=0 WRITE OPERATION
	 WBYTE(IOPIN1,LCD_DATA,data);//EN=1
	 SSETBIT(IOSET0,EN);
	 delay_us(1);
	 SCLRBIT(IOCLR0,EN);//EN=0
	 delay_ms(2);//internal process
}
void CmdLCD(u8 cmd)
{
	 SCLRBIT(IOCLR0,RS);//RS=0 COMMAND REGISTER SELECTED
	 WriteLCD(cmd);
}
void CharLCD(u8 ascii)
{
	  SSETBIT(IOSET0,RS);//RS=0 COMMAND REG SELECTED
	  WriteLCD(ascii);
}
void Init_LCD(void)
{
	 WBYTE(IODIR1,LCD_DATA,255);//p0.8 to p0.15 as o/p
	 SETBIT(IODIR0,RS);
	// SETBIT(IODIR0,RW);
	 SETBIT(IODIR0,EN);

	 delay_us(15);
	 CmdLCD(MODE_8BIT_1LINE);
	 delay_us(5);
	 CmdLCD(MODE_8BIT_1LINE);
	 delay_ms(100);
     CmdLCD(MODE_8BIT_1LINE);
	 CmdLCD(MODE_8BIT_2LINE);
	 CmdLCD(DISP_ON);
	 CmdLCD(CLEAR_LCD);
	 CmdLCD(SHIFT_CUR_RIGHT);
}
void StrLCD(char *p)
{
	while(*p)
	 	CharLCD(*p++);
}
void U32LCD(u32 n) //For display string
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n/=10;
		}
	}
	for(--i; i>=0; i--)
		CharLCD(a[i]);
}
void S32LCD(s32 n)//for display negative int
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
		U32LCD(n);
	}
}
void F32LCD(f32 fnum,u8 nDP)//for display float value
{
	u32 n;
	s32 i;
	if(fnum<0)
	{
		CharLCD('-');
	}
	else
	{
		n=fnum;
		U32LCD(n);
		CharLCD('.');
		for(i=0; i<nDP; i++)
		{
			fnum=(fnum-n)*10;
			n=fnum;
			CharLCD(n+48);
		}
	}
}
void HEXLCD(u32 n)
{
	u8 a[8],rem;
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');	
	}
	else
	{
		while(n)
		{
			rem=n%16;
			(rem<10)?(rem+=48):(rem+=55);
			 a[i++]=rem;
			 n/=16;
		}
		for(--i; i>=0; i--)//for reversing the loop
		{
			CharLCD(a[i]);
		}

	}
}
void OCTALCD(u32 n)
{
	u8 a[8],rem;
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');	
	}
	else
	{
		while(n)
		{
			rem=n%8;
			(rem<10)?(rem+=48):(rem+=55);
			 a[i++]=rem;
			 n/=8;
		}
		for(--i; i>=0; i--)//for reversing the loop
		{
			CharLCD(a[i]);
		}

	}
}
void BinLCD(u32 n,u8 nbd)//for binary
{
	s32 i;
	for(i=nbd-1; i>=0; i--)
	{
		CharLCD(((n>>i)&1)+48);
	}
}
void BuildCGRAM(u8 *p,u8 nb)
{
	s32 i;
	CmdLCD(GOTO_CGRAM);
	for(i=0; i<=nb; i++)
	{
		CharLCD(p[i]);
	}
	CmdLCD(GOTO_LINE2_POS0);
}
