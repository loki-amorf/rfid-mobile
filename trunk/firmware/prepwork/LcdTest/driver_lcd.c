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

void graphTest(void)
{
	int x, y;
	
    writeInst(LCD_EXT_INST_SET); // extended instruction set
    writeInst(LCD_INSTEX_GRAOFF); // graphics off
    
    x = 15;
    for (y = 0; y < 32; ++y) {
		writeInst(0x80 + y);
		writeInst(0x80 + x);
		writeData(0xFF);
		writeData(0xFF);
    }
    
    writeInst(LCD_INSTEX_GRAON); // graphics on
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
    	case 0:
    	break;
    	
    	case 1:
    	row = 2;
    	break;
    	
    	case 2:
    	row = 1;
    	break;
    	
    	case 3:
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

void invertLine(int line)
{
	writeInst(LCD_EXT_INST_SET);
	writeInst(0x04 + line);
}

void initGraph(void)
{
	// clear GDRAM
	int x, y;
	writeInst(LCD_EXT_INST_SET);
	writeInst(LCD_INSTEX_GRAOFF);
    for (x = 0; x < 16; ++x) {
	    for (y = 0; y < 32; ++y) {
			writeInst(0x80 + y);
			writeInst(0x80 + x);
			writeData(0x00);
			writeData(0x00);
	    }
    }
	// graphics on
	writeInst(LCD_INSTEX_GRAON);
}
