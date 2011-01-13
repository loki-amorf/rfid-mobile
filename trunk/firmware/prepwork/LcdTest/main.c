#include <msp430x24x.h>
#include <string.h>
#include "helper_bits.h"
#include "driver_lcd.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();

	//setCharPos(0, 0);
	//writeStr("Line-0");
	//setCharPos(1, 0);
	//writeStr("Line-1");
	//setCharPos(2, 0);
	//writeStr("Line-2");
    //setCharPos(3, 0);
	//writeStr("Line-3");

   	graphTest();
	
	while (1)
		; // NULL
}
