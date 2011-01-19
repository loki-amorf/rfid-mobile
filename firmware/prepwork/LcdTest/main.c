#include <msp430x24x.h>
#include <string.h>
#include "helpers.h"
#include "driver_lcd.h"
#include "res_picture.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	initLcd();

    // main menu background and icons
    drawPicture(res_appleLogo, sizeof(res_appleLogo));
    delay(2000);
    clearGraph();
    drawPicture(res_mainmenu, sizeof(res_mainmenu));
    
    // main menu chars
    setCharPos(0, 3);
    writeStr("MENU");
    setCharPos(3, 0);
    writeStr(" Mail");
    setCharPos(3, 3);
    writeStr(" NFC");
    setCharPos(3, 6);
    writeStr("Game");
    
    do {
        ;
    } while (1);
}
