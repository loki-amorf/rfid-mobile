#include "helpers.h"
#include "driver_lcd.h"

void delay(int x)
{
    int i, j;
    
    i = x;
    do {
        j = 100;
        do {
            --j;
        } while (j != 0);
    } while (--i != 0);
}

void error(char *str)
{
    initLcd();
    setCharPos(0, 0);
    writeStr("ERROR: ");
    writeStr("str");
    do {
        ;
    } while (1);
}
