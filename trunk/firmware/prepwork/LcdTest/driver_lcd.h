#ifndef DRIVER_LCD_H_
#define DRIVER_LCD_H_


#include <msp430x24x.h>
#include "helpers.h"

// pins assignment
#define LCD_CTRL	        (P5OUT)
#define LCD_OUT		        (P4OUT)
#define LCD_IN		        (P4IN)
#define LCD_RS		        (0x01)
#define LCD_RW		        (0x08)
#define LCD_E		        (0x10)
#define LCD_PSB		        (0x20)
#define LCD_nRET	        (0x40)

// position in register
#define LCD_BF		        (0x80)

// LCD pin operations
#define LCD_DPIN_DIR_OUT    (P4DIR = 0xFF)
#define LCD_DPIN_DIR_IN     (P4DIR = 0x00)
#define LCD_CPIN_DIR_OUT    (P5DIR |= (LCD_RS | LCD_RW | LCD_E | LCD_PSB | LCD_nRET))

// LCD size
#define LCD_CHAR_LINES      (4)
#define LCD_CHAR_COLS       (8)
#define LCD_PIXEL_X         (128)
#define LCD_PIXEL_Y         (64)

// address
#define LCD_CHAR_BASEADDR   (0x80)

// LCD INSTRUCTIONS
// choose instruction set
#define LCD_BASIC_INST_SET  (0x30)
#define LCD_EXT_INST_SET    (0x36)

// Basic Instructions Set
// clear and reset
#define LCD_INST_CLR        (0x01)
#define LCD_INST_RSTADR     (0x02)
        
// entry mode       
#define LCD_INST_LTR        (0x04)
#define LCD_INST_RTL        (0x06)
        
// display mode     
#define LCD_INST_DOCOBO     (0x0F)
#define LCD_INST_DOCOBF     (0x0E)
#define LCD_INST_DOCFBF     (0x0C)
#define LCD_INST_DFCFBF     (0x08)

// cursor and display shift
#define LCD_INST_CURL       (0x10)
#define LCD_INST_CURR       (0x14)
#define LCD_INST_DISPL      (0x18)
#define LCD_INST_DISPR      (0x1C)

// Extended Instructions Set
#define LCD_INSTEX_STDBY    (0x01)
#define LCD_INSTEX_SCREN    (0x03)
#define LCD_INSTEX_IRAMEN   (0x02)
#define LCD_INSTEX_IV13     (0x04)
#define LCD_INSTEX_IV24     (0x05)
#define LCD_INSTEX_SLEEP    (0x08)
#define LCD_INSTEX_GETUP    (0x0C)
#define LCD_INSTEX_GRAON    (0x36)
#define LCD_INSTEX_GRAOFF   (0x34)

// write instruction to DB0~7
void writeInst(char inst);

// write data to DB0~7
void writeData(char data);

// read RAM
uint8 readRam(void);

// initialize LCD display
void initLcd(void);

// clear LCD display
void clearLcd(void);

// write string given a string and its length
void writeStr(char *str);

// write first n bytes of str
void writeNStr(char *str, int n);

// set char position given line and column (0-indexed!)
void setCharPos(int row, int col);

// invert line
void invertLine(int line);

// clear GDRAM and enable graphics
void clearGraph(void);

// enter ex_inst and turn off graphics show
void beginDraw(void);

// turn on graphics show and leave ex_inst
void endDraw(void);

// draw single pixel, low efficiency
void drawPixel(uint8 x0, uint8 y0);

// connect (x0, y0) and (x1, y1) using straight line
void drawLine(int x0, int y0, int x1, int y1);

// draw picture to LCD using a special encoding style
void drawPicture(const uint8 *pic, int totalLen);

#endif /*DRIVER_LCD_H_*/
