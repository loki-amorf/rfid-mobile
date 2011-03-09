#include <msp430x14x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"
#include "res_picture.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();
    
	drawPicture(res_appleLogo, sizeof(res_appleLogo));
	
    do {
        ;
    } while (1);
}
