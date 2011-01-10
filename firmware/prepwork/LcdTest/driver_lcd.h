#ifndef DRIVER_LCD_H_
#define DRIVER_LCD_H_


#include <msp430x24x.h>

#define LCD_CTRL	P5OUT
#define LCD_OUT		P4OUT
#define LCD_IN		P4IN

#define LCD_RS		0x00
#define LCD_RW		0x04
#define LCD_E		0x08
#define LCD_PSB		0x10
#define LCD_nRET	0x20
#define LCD_BF		0x80

#define LCD_PIN_DIR_OUT (P4DIR = 0xFF)
#define LCD_PIN_DIR_IN (P4DIR = 0x00)

// write instruction to DB0~7
void writeInst(char inst);

// initialize LCD display
void initLcd(void);

// 

#endif /*DRIVER_LCD_H_*/
