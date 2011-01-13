/*
 * serial_port_driver.cpp
 */
#include "msp430x24x.h"
#include "serial_port_driver.h"

int SPDI_Initial(int port3, char br)
{
    P3SEL &= 0x0F;
    P3SEL |= port3;

    if (port3 & URAT0) {
	    switch (br) {
	    case BAUDRATE115k:
	        UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	        UCA0BR0 = 8;                              // 1MHz 115200
	        UCA0BR1 = 0;                              // 1MHz 115200
	        UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
	        break;
	    case BAUDRATE9600:
	        UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	        UCA0BR0 = 6;                              // 1MHz 9600
	        UCA0BR1 = 0;                              // 1MHz 9600
	        UCA0MCTL = UCBRF3 + UCOS16;               // Modln UCBRSx=1, over sampling
	        break;
	    default:
	        return -1;
	    }
    
        UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }
    if (port3 & URAT1) {
        switch (br) {
        case BAUDRATE115k:
            UCA1CTL1 |= UCSSEL_2;                     // SMCLK
            UCA1BR0 = 8;                              // 1MHz 115200
            UCA1BR1 = 0;                              // 1MHz 115200
            UCA1MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
            break;
        case BAUDRATE9600:
            UCA1CTL1 |= UCSSEL_2;                     // SMCLK
            UCA1BR0 = 6;                              // 1MHz 9600
            UCA1BR1 = 0;                              // 1MHz 9600
            UCA1MCTL = UCBRF3 + UCOS16;               // Modln UCBRSx=1, over sampling
            break;
        default:
            return -1;
        }
    
        UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    }
    
    return 0;
}
