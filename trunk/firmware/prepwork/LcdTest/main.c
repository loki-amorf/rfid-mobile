#include <msp430x24x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();

    int i;
    
//#define LINE_TEST_1
//#define LINE_TEST_2

#ifdef LINE_TEST_1
    for (i = 0; i < LCD_PIXEL_Y; i += 5) {
        drawLine(0, i, LCD_PIXEL_X - 1, LCD_PIXEL_Y - i);
    }
#endif

#ifdef LINE_TEST_2
    for (i = 0; i < LCD_PIXEL_Y; ++i) {
        drawLine(0, i, LCD_PIXEL_X - 1, i);
    }
#endif

    do {
        ;
    } while (1);
}
