#include "driver_lcd.h"
#include "helper_bits.h"

void writeInst(char inst)
{
	// wait for BF set
	BIT_SET(LCD_CTRL, LCD_RS);
	BIT_SET(LCD_CTRL, LCD_RW);
	LCD_PIN_DIR_IN;
	BIT_SET(LCD_CTRL, LCD_E);
	do {
		; // Null
	} while (LCD_IN & LCD_BF);

	// write to pins
	LCD_PIN_DIR_OUT;
	BIT_UNSET(LCD_CTRL, LCD_RW);
	LCD_OUT = inst;
	BIT_SET(LCD_CTRL, LCD_E);
	BIT_UNSET(LCD_CTRL, LCD_E);
}

void initLcd(void)
{	
	// enable display, cursor and cursor blink
	writeInst(0x0F);
	
	
}
