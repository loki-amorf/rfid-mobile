#include "driver_lcd.h"
#include "helpers.h"

void writeInst(char inst)
{
    // wait for BF set
    LCD_DPIN_DIR_IN;
    BIT_UNSET(LCD_CTRL, LCD_RS); // instruction
    BIT_SET(LCD_CTRL, LCD_RW); // read
    BIT_UNSET(LCD_CTRL, LCD_E);
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
    BIT_UNSET(LCD_CTRL, LCD_E);
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

uint8 readRam(void)
{
    int i;
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
    
    // wait for a while!
    i = 1;
    do {
        --i;
    } while (i != 0);
    return LCD_IN;
}

void setCharPos(int row, int col)
{
    uint8 ddramPos;

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

void writeNStr(char *str, int n)
{
    int i;
    
    for (i = 0; i < n; ++i) {
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
        LCD_OUT = str[i];
        BIT_UNSET(LCD_CTRL, LCD_E); // write to
    }
}

void writeNChars(char c, int n)
{
    int i;
    for (i = 0; i < n; ++i) {
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
        LCD_OUT = c;
        BIT_UNSET(LCD_CTRL, LCD_E); // write to
    }
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

    // choose basic instrucion set
    writeInst(LCD_BASIC_INST_SET);
    
    // display on, cursor off, blink off
    writeInst(LCD_INST_DOCFBF);
}

void clearLcd(void)
{
	writeInst(LCD_BASIC_INST_SET);
    writeInst(LCD_INST_CLR);
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

void initGraph(void)
{
	// clear GDRAM
	int lcd_y;
    beginDraw();
    for (lcd_y = 0; lcd_y < 32; ++lcd_y) {
		writeInst(0x80 + lcd_y);
        writeInst(0x80);
        writeNChars(0x00, 32);
    }
	endDraw();
}

void fillCharLine(int charLine, char stuff)
{
    int lcd_x0, lcd_y0;
    int lcd_y;

    lcd_x0 = (charLine >> 1) << 3;
    lcd_y0 = (charLine & 0x0001) << 4;
    
    beginDraw();
    for (lcd_y = lcd_y0; lcd_y < lcd_y0 + 16; ++lcd_y) {
        writeInst(0x80 + lcd_y);
        writeInst(0x80 + lcd_x0);
        writeNChars(stuff, 16);
    }
    endDraw();
}

void drawPixel(uint8 x0, uint8 y0)
{
    uint8 lcd_x, lcd_y;
    uint8 rel_x;
    uint8 curRam_0, curRam_1;
    
    beginDraw();
    
    // user x,y to lcd x,y
    lcd_y = y0 & 0x1F; // lcd_y = y % 32;
    rel_x = x0 & 0x0F; // rel_x = x % 16
    lcd_x = ((y0 >> 5) << 3) + (x0 >> 4); // lcd_x = (y0 / 32) * 8 + x0 / 16
    
    // set position
    writeInst(0x80 + lcd_y);
    writeInst(0x80 + lcd_x);
    
    // read ram
    curRam_0 = readRam();
    curRam_1 = readRam();
    
    // modify ram
    if (rel_x > 7) { // rel_x > 7, modify curRam_1
        curRam_1 |= (0x80 >> (rel_x - 8));
    }
    else {
        curRam_0 |= (0x80 >> rel_x);
    }
    
    // write back to ram
    writeInst(0x80 + lcd_y);
    writeInst(0x80 + lcd_x);
    writeData(curRam_0);
    writeData(curRam_1);
    
    endDraw();
}

void drawLine(int x0, int y0, int x1, int y1)
{
    int x, y;
    int dx, dy;
    int sx, sy;
    int err, e2;
    
    x = x0;
    y = y0;
    dx = x1 - x0; if (dx < 0) dx = -dx; // dx = abs(x1 - x0)
    dy = y1 - y0; if (dy < 0) dy = -dy; // dy = abs(y1 - y0)
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = dx - dy;
    
    while (1) {
        drawPixel(x, y);
        if (x == x1 && y == y1) break;
        e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void drawPicture(const uint8 *pic, int totalLen)
{
    int index = 0;
    const uint8 *pCurr = pic;
    uint8 cnt;
    
    beginDraw();
    while (index < totalLen) {
        // first 2 bytes as position
        writeInst(0x80 + *(pCurr + 0));
        writeInst(0x80 + *(pCurr + 1));
        
        // 3rd byte as counter
        cnt = *(pCurr + 2);
        
        // write cnt bytes to ram
        writeNStr((char *)(pCurr + 3), cnt);
        
        // add index, move pCurr
        index += (cnt + 3);
        pCurr += (cnt + 3);
    }
    endDraw();
}
