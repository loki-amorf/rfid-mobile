/*
 * RFID-mobile
 * 
 * This is the keyboard module driver
 * 
 */

#ifndef DRIVER_KBD_H_
#define DRIVER_KBD_H_

#include <msp430x14x.h>

// Msp pins
#define KBD_IN 		(P1IN)
#define KBD_OUT 	(P1OUT)
#define KBD_DIR		(P1DIR)

// Keys assign
#define KBD_KEY_0		(0x0001 << 13)
#define KBD_KEY_1		(0x0001 << 0) 
#define KBD_KEY_2		(0x0001 << 1) 
#define KBD_KEY_3		(0x0001 << 2) 
#define KBD_KEY_4		(0x0001 << 4) 
#define KBD_KEY_5		(0x0001 << 5) 
#define KBD_KEY_6		(0x0001 << 6) 
#define KBD_KEY_7		(0x0001 << 8) 
#define KBD_KEY_8		(0x0001 << 9) 
#define KBD_KEY_9		(0x0001 << 10) 
#define KBD_KEY_SHARP	(0x0001 << 14)
#define KBD_KEY_ASTER	(0x0001 << 12)
#define KBD_KEY_UP		(0x0001 << 3)
#define KBD_KEY_DOWN	(0x0001 << 7)
#define KBD_KEY_LEFT	(0x0001 << 11) // also used as CONFIRM
#define KBD_KEY_RIGHT	(0x0001 << 15) // also used as BACK

// Do keyboard scan
void kbd_scanner(void);

// call back funtions used as interface
void kbd_keyPressed(unsigned short keyNum);

#endif /*DRIVER_KBD_H_*/
