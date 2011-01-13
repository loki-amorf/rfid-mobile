//******************************************************************************

//******************************************************************************
#include "msp430x24x.h"
#include "serial_port_driver.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
  {  
    while(1);                               // If calibration constants erased
                                            // do not load, trap CPU!!
  }   
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

  SPDI_Initial(URATALL, BAUDRATE115k);
  
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  UC1IE |= UCA1RXIE;                        // Enable USCI_A1 RX interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
  
  while(1)
    ;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2 & UCA0TXIFG & UCA1TXIFG));  // USCI_A0 & USCI_A1 TX buffer ready?
  UCA1TXBUF = UCA0RXBUF;                    // TX -> RXed character
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR(void)
{
  while (!(IFG2 & UCA0TXIFG));  // USCI_A0 & USCI_A1 TX buffer ready?
  UCA0TXBUF = UCA1RXBUF;                    // TX -> RXed character
}
