//******************************************************************************

//******************************************************************************
#include "msp430x24x.h"
#include "serial_port_driver.h"

// deal with the recieve data when RX0 interrupt occur.
void RX0IOccured(uchar buf)
{
    while (!(SPDI_IsReadyToWrite(URAT0)&SPDI_IsReadyToWrite(URAT0)))
        ;                               // USCI_A0 & USCI_A1 TX buffer ready?
    UCA1TXBUF = buf;                    // TX -> RXed character
    UCA0TXBUF = buf;                    // TX -> RXed character
}
// deal with the recieve data when RX1 interrupt occur.
void RX1IOccured(uchar buf)
{
    while (!SPDI_IsReadyToWrite(URAT0))
        ;                               // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA1RXBUF;              // TX -> RXed character
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    if (SPDI_Initial() == SP_ERR_CAL_CONST_ERASED)                                     
    {
        while(1);                               // If calibration constants erased
                                            // do not load, trap CPU!!
    }

    SPDI_SetRX0FunP(RX0IOccured);
    SPDI_SetRX1FunP(RX1IOccured);

    SPDI_Open(URAT0, BAUDRATE115k);
    SPDI_Open(URAT1, BAUDRATE115k);

    while(1)
        ;
}
