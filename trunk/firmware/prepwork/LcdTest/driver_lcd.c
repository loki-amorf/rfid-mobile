#include "driver_lcd.h"
#include "helpers.h"

void writeInst(char inst)
{
    // wait for BF set
    LCD_DPIN_DIR_IN;
    BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
    BIT_SET(LCD_CTRL, LCD_RW); // read
    // BIT_UNSET(LCD_CTRL, LCD_E);
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

uint8 readRam(void)
{
    // wait for BF set
    LCD_DPIN_DIR_IN;
    BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
    BIT_SET(LCD_CTRL, LCD_RW); // read
    BIT_SET(LCD_CTRL, LCD_E);
    do {
        ; //NULL
    } while (LCD_IN & LCD_BF);

    BIT_SET(LCD_CTRL, LCD_RS); // data
    BIT_UNSET(LCD_CTRL, LCD_E);
    BIT_SET(LCD_CTRL, LCD_E);
    return LCD_IN;
}

void writeData(char data)
{
    // wait for BF set
    LCD_DPIN_DIR_IN;
    BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
    BIT_SET(LCD_CTRL, LCD_RW); // read
    // BIT_UNSET(LCD_CTRL, LCD_E);
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

void writeStr(char *str)
{
    char *pCurr = str;

	while (*pCurr != '\0')
	{
        // wait for BF set
        LCD_DPIN_DIR_IN;
        BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
        BIT_SET(LCD_CTRL, LCD_RW); // read
        BIT_SET(LCD_CTRL, LCD_E); // enable read
        do {
            ;
        } while (LCD_IN & LCD_BF);

        // write one char
        BIT_SET(LCD_CTRL, LCD_RS); // data
        BIT_UNSET(LCD_CTRL, LCD_RW); // write
        LCD_DPIN_DIR_OUT;
        LCD_OUT = *pCurr;
        BIT_UNSET(LCD_CTRL, LCD_E); // write to
        
        // move to next
        ++pCurr;
    }
}

void setCharPos(int row, int col)
{
    char ddramPos;

    switch (row) {
    	case 1:
    	row = 2;
    	break;
    	
    	case 2:
    	row = 1;
    	break;
    	
    	default:
    	break;
    }
    ddramPos = LCD_CHAR_BASEADDR + col + row * LCD_CHAR_COLS;
    writeInst(ddramPos);
}

void initLcd(void)
{	
    // control pins direction out
    LCD_CPIN_DIR_OUT;

    // choose parallel mode
    BIT_SET(LCD_CTRL, LCD_PSB);
    
    // clear display
    clearLcd();
    
    // clear GDRAM and enable graph display
    initGraph();

    // display on, cursor off, blink off
    writeInst(LCD_BASIC_INST_SET);
    writeInst(LCD_INST_DOCFBF);
}

void clearLcd(void)
{
	writeInst(LCD_BASIC_INST_SET);
    writeInst(LCD_INST_CLR);
}

void invertLine(int line) // need modification
{
	writeInst(LCD_EXT_INST_SET);
	writeInst(0x04 + line);
}

uint8 xyToLcdXY(uint8 x0, uint8 y0, uint8 *pLcd_x, uint8 *pLcd_y)
{
    if (y0 & 0x20) { // y0 > 31
        *pLcd_y = y0 & (~0x20); // lcd_y = y0 - 32;
        *pLcd_x = (x0 >> 4) | 0x08; // lcd_x = x0 / 16 + 8;
        return (x0 & 0x0F); // lower 4 bits
    }
    else {
        *pLcd_y = x0;
        *pLcd_x = (x0 >> 4);
        return (x0 & 0x0F);
    }
}

void initGraph(void)
{
	// clear GDRAM
	int x, y;
    beginDraw();
    for (x = 0; x < 16; ++x) {
	    for (y = 0; y < 32; ++y) {
			writeInst(0x80 + y);
			writeInst(0x80 + x);
			writeData(0x00);
			writeData(0x00);
	    }
    }
	endDraw();
}

void beginDraw(void)
{
    writeInst(LCD_EXT_INST_SET);
    writeInst(LCD_INSTEX_GRAOFF);
}

void endDraw(void)
{
    writeInst(LCD_INSTEX_GRAON);
    writeInst(LCD_BASIC_INST_SET);
}

void graphTest(void)
{
    int x, y;
    
    beginDraw();
    x = 15;
    for (y = 0; y < 32; ++y) {
        writeInst(0x80 + y);
        writeInst(0x80 + x);
        writeData(0xAA);
        writeData(0xFE);
    }
    endDraw();
}
