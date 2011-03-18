/*
 * RFID-mobile
 * 
 * This is the keyboard module driver
 * 
 */

#ifndef DRIVER_KBD_H_
#define DRIVER_KBD_H_

#include <msp430x14x.h>

#define KBD_IN 		(P1IN)
#define KBD_OUT 	(P1OUT)
#define KBD_DIR		(P1DIR)

// Do keyboard scan
void kbd_scanner(void);

// call back funtions used as interface
void kbd_keyPressed(unsigned short keyNum);

#endif /*DRIVER_KBD_H_*/
