#ifndef DRIVER_LCD_H_
#define DRIVER_LCD_H_


#include <msp430x24x.h>

#define LCD_CTRL	P5OUT
#define LCD_OUT		P4OUT
#define LCD_IN		P4IN

#define LCD_RS		0x01
#define LCD_RW		0x08
#define LCD_E		0x10
#define LCD_PSB		0x20
#define LCD_nRET	0x40

#define LCD_BF		0x80

#define LCD_DPIN_DIR_OUT (P4DIR = 0xFF)
#define LCD_DPIN_DIR_IN (P4DIR = 0x00)
#define LCD_CPIN_DIR_OUT (P5DIR |= (LCD_RS | LCD_RW | LCD_E | LCD_PSB | LCD_nRET))

#define LCD_CHAR_LINES 4
#define LCD_CHAR_COLS 8

// write instruction to DB0~7
void writeInst(char inst);

// write data to DB0~7
void writeData(char data);

// initialize LCD display
void initLcd(void);

// clear LCD display
void clearLcd(void);

// invert display (not functioning)
void invertLcd(void);

// write string given a string and its length
void writeStr(char *str, int len);

// set position given line and column (0-indexed!)
void setPos(int row, int col);


#endif /*DRIVER_LCD_H_*/
