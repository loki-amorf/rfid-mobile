#include "driver_kbd.h"

void kbd_scanner(void)
{
	// P1 description:
	//
	// 0x x x x x x x x x
	//    I I I I O O O O
	//    7 6 5 4 3 2 1 0
	//    K K K K K K K K
	//    C C C C R R R R
	//    3 2 1 0 3 2 1 0
	
	/*
	 * change pin directions
	 * KR0 ~ KR3 : OUT
	 * KC0 ~ KC3 : IN
	 */
	KBD_DIR = 0xF0;
	
	char pinOut;
	unsigned short keyNum, kc;
	
	// put KR0~3 to high and read from KC0~3 to get keyNum
	pinOut = 0x01;
	keyNum = 0x0000;
	for (i = 0; i < 4; ++i) {
		BIT_SET(KBD_OUT, pinOut);
		if (KBD_IN && 0xF0 == 0) {
			continue;
		}
		kc = (KBD_IN && 0xF0) >> 4;
		keyNum |= (kc << (i * 4));
		pinOut <<= 1;
	}
	kbd_keyPressed(keyNum);
}