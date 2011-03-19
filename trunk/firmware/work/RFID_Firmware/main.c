#include <msp430x14x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"
#include "driver_kbd.h"
#include "res_picture.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	// LCD driver
	initLcd();

	// Set timer for keyboard scan
	CCTL0 = CCIE; // CCR0 interrupt enabled
	CCR0 = 10000; // 10ms
	TACTL = TASSEL_2 + MC_1; // SMCLK, contmode
	_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
	
    do {
        ;
    } while (1);
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	kbd_scanner();
}
