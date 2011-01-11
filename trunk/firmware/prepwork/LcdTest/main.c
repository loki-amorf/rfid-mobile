#include <msp430x24x.h>
#include "helper_bits.h"
#include "driver_lcd.h"

void main(void)
{
	char ii;
	
	initLcd();
	
	for (ii = 0x02; ii <= 0x7F; ++ii) {
		writeData(ii);
	}
	
	while (1)
	;
}
