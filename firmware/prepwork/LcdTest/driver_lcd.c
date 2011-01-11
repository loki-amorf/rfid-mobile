#include "driver_lcd.h"
#include "helper_bits.h"

void writeInst(char inst)
{
	// wait for BF set
	LCD_DPIN_DIR_IN;
	BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
	BIT_SET(LCD_CTRL, LCD_RW); // read
	BIT_SET(LCD_CTRL, LCD_E);
	do {
		; //NULL
	} while (LCD_IN & LCD_BF);

	// write to pins
	LCD_DPIN_DIR_OUT;
	BIT_UNSET(LCD_CTRL, LCD_RW); // write mode
	LCD_OUT = inst;
	BIT_UNSET(LCD_CTRL, LCD_E); // write to
}

void writeData(char data)
{
	// wait for BF set
	LCD_DPIN_DIR_IN;
	BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
	BIT_SET(LCD_CTRL, LCD_RW); // read
	BIT_SET(LCD_CTRL, LCD_E);
	do {
		; //NULL
	} while (LCD_IN & LCD_BF);

	// write to pins
	LCD_DPIN_DIR_OUT;
	BIT_SET(LCD_CTRL, LCD_RS); // data
	BIT_UNSET(LCD_CTRL, LCD_RW); // write mode
	LCD_OUT = data;
	BIT_UNSET(LCD_CTRL, LCD_E); // write to
}

void initLcd(void)
{	
	// control pins direction out
	LCD_CPIN_DIR_OUT;
	
	// choose parallel mode
	BIT_SET(LCD_CTRL, LCD_PSB);
	
	// clear display
	writeInst(0x01);
	
	// enable display, diable cursor
	writeInst(0x0C);
}
