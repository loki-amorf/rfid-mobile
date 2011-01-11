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

// initialize LCD display
void initLcd(void);

// write instruction to DB0~7
void writeInst(char inst);

// write data to DB0~7
void writeData(char data);

#endif /*DRIVER_LCD_H_*/
