#include "lcd.h"
#include "menu.h"

void show_menu()
{
    CmdLCD(0x80);
    StrLCD("1:Add 2:Del");
    CmdLCD(0xC0);
    StrLCD("3:Pay");
}
