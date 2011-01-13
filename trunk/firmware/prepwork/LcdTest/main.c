#include <msp430x24x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"
#include "system_gui.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();

    setCharPos(3, 0);
    writeStr("Line-3");

   	graphTest();
    
    drawPixel(3, 3);
    drawLine(0, 0, 20, 20);
	
	while (1)
		; // NULL
}
