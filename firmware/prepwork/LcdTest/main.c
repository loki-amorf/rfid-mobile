#include <msp430x24x.h>
#include <string.h>
#include "helper_bits.h"
#include "driver_lcd.h"

void main(void)
{
	initLcd();
	
	setPos(2, 1);
	char *str = "Hello World!";
	writeStr(str, strlen(str));
	
	while (1)
		; // NULL
}
