#include <msp430x24x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"
#include "res_picture.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();
    
//#define LINE_TEST_1
//#define LINE_TEST_2
#define PIC_TEST_1

#ifdef LINE_TEST_1
    int i;
    
    for (i = 0; i < LCD_PIXEL_Y; i += 5) {
        drawLine(0, i, LCD_PIXEL_X - 1, LCD_PIXEL_Y - i);
    }
#endif

#ifdef LINE_TEST_2
    int i;
    
    for (i = 0; i < LCD_PIXEL_Y; ++i) {
        drawLine(0, i, LCD_PIXEL_X - 1, i);
    }
#endif

#ifdef PIC_TEST_1
    drawPicture(res_pic1, sizeof(res_pic1));
#endif

    do {
        ;
    } while (1);
}
