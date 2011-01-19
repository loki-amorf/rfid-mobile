/*
 * serial_port_driver.cpp
 */
#include "serial_port_driver.h"

// define the buffer length of serial port.
#define BUF_LENGTH  40

// define empty functions for function points.
void emptyFunUChar(char x) {}
void emptyFunVoid() {}

// initial the static function points with empty function.
static RXIFunP rx0FuncP;
static RXIFunP rx1FuncP;
static TXIFunP tx0FuncP;
static TXIFunP tx1FuncP;


// define recieve exchange buffer variables.
static char rx0BufferAddress[BUF_LENGTH];
static char *rx0BufferPosition;
// define transfer exchange buffer variables.
static char tx0BufferAddress[BUF_LENGTH];
static char *tx0BufferPosition;
// define recieve exchange buffer variables.
static char rx1BufferAddress[BUF_LENGTH];
static char *rx1BufferPosition;
// define transfer exchange buffer variables.
static char tx1BufferAddress[BUF_LENGTH];
static char *tx1BufferPosition;

/*
 * Name: SPDI_Initial
 * This function must be calling first to initialize the environment.
 */
int SPDI_Initial()
{
    // return if calibration constants erased
    if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
    {
        return SP_ERR_CAL_CONST_ERASED;
    }
    BCSCTL1 = CALBC1_1MHZ;      // Set DCO
    DCOCTL = CALDCO_1MHZ;
    
    // initial the static function points with empty function.
    rx0FuncP = emptyFunUChar;
    rx1FuncP = emptyFunUChar;
    tx0FuncP = emptyFunVoid;
    tx1FuncP = emptyFunVoid;
    
    rx0BufferPosition = rx0BufferAddress;
    rx1BufferPosition = rx1BufferAddress;
    tx0BufferPosition = tx0BufferAddress;
    tx1BufferPosition = tx1BufferAddress;
    
    return SP_NORMAL;           // normal.
}

/*
 * This four function assign the function points to the given function,
 * so the given function will be callback when interrupts occur.
 */
void SPDI_SetRX0FunP(RXIFunP fp) {
    rx0FuncP = fp;
}
void SPDI_SetRX1FunP(RXIFunP fp) {
    rx1FuncP = fp;
}
void SPDI_SetTX0FunP(TXIFunP fp) {
    tx0FuncP = fp;
}
void SPDI_SetTX1FunP(TXIFunP fp) {
    tx1FuncP = fp;
}

int SPDI_SetBaudRate(portType port, int br)
{
    switch (port) {
    case URAT0:
        switch (br) {
        case BAUDRATE115k:
            UCA0CTL1 |= UCSSEL_2;              // SMCLK
            UCA0BR0 = 8;                       // 1MHz 115200
            UCA0BR1 = 0;                       // 1MHz 115200
            UCA0MCTL = UCBRS2 + UCBRS0;        // Modulation UCBRSx = 5
            break;
        case BAUDRATE9600:
            UCA0CTL1 |= UCSSEL_2;              // SMCLK
            UCA0BR0 = 6;                       // 1MHz 9600
            UCA0BR1 = 0;                       // 1MHz 9600
            UCA0MCTL = UCBRF3 + UCOS16;        // Modln UCBRSx=1, over sampling
            break;
        default:
            return SP_ERR_NO_BAUDRATE;
        }
        break;
    case URAT1:
        switch (br) {
        case BAUDRATE115k:
            UCA1CTL1 |= UCSSEL_2;               // SMCLK
            UCA1BR0 = 8;                        // 1MHz 115200
            UCA1BR1 = 0;                        // 1MHz 115200
            UCA1MCTL = UCBRS2 + UCBRS0;         // Modulation UCBRSx = 5
            break;
        case BAUDRATE9600:
            UCA1CTL1 |= UCSSEL_2;               // SMCLK
            UCA1BR0 = 6;                        // 1MHz 9600
            UCA1BR1 = 0;                        // 1MHz 9600
            UCA1MCTL = UCBRF3 + UCOS16;         // Modln UCBRSx=1, over sampling
            break;
        default:
            return SP_ERR_NO_BAUDRATE;
        }
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }

    return SP_NORMAL;
}

/*
 * Name: SPDI_Open
 *
 * Open the serial port specified by var 'port'. Note this function's
 * has appendding effect. That is to say, you can recalling this function
 * to reopen a serial port.
 */
int SPDI_Open(portType port, int br)
{
    SPDI_SetBaudRate(port, br);

    P3SEL |= port;

    switch (port) {
    // if check URAT0
    case URAT0:
        UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
        UC0IE |= UCA0RXIE;                      // Enable USCI_A0 recieve exchange interrupt
        break;
    // if check URAT1
    case URAT1:
        UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
        UC1IE |= UCA1RXIE;                      // Enable USCI_A1 recieve exchange interrupt
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }

    __bis_SR_register(GIE);
    return SP_NORMAL;
}

int SPDI_Close(portType port)
{
    switch (port) {
    case URAT0:
        UC0IE &= ~(UCA0RXIE | UCA0TXIE);         // Disable USCI_A0 interrupt
        break;
    case URAT1:
        UC1IE &= ~(UCA1RXIE | UCA1TXIE);         // Disable USCI_A1 interrupt
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }
    
    return SP_NORMAL;
}

/* 
 * Name: SPDI_IsReadyToWrite
 *  Return TRUE if the UCAxTXBUF is empty.
 */
int SPDI_IsReadyToWrite(portType port)
{
    switch (port) {
    case URAT0:
        // if not ready, Open the transfer interrupt and return FALSE
        if (tx0BufferAddress != tx0BufferPosition) {
            UC0IE |= UCA0TXIE;
            // force open interrupt enable to make transfer interrupt available.
            __bis_SR_register(GIE);
            return FALSE;
        }
        break;
    case URAT1:
        // if not ready, Open the transfer interrupt and return FALSE
        if (tx1BufferAddress != tx1BufferPosition) {
            UC1IE |= UCA1TXIE;
            // force open interrupt enable to make transfer interrupt available.
            __bis_SR_register(GIE);
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;            // if all ready, return TRUE;
}
/* Name: SPDI_IsReadyToRead
 * Return TRUE if the UCAxRXBUF has received a complete character
 */
int SPDI_IsReadyToRead(portType port)
{
    switch (port) {
    case URAT0:
        if (!(UC0IFG & UCA0RXIFG))
            return FALSE;   // if not ready, return FALSE
        break;
    case URAT1:
        if (!(UC0IFG & UCA1RXIFG))
            return FALSE;   // if not ready, return FALSE
        break;
    default:
        return FALSE;
    }

    return TRUE;            // if all ready, return TRUE;
}


/*
 * Name: SPDI_ReadAll
 * Read the buffer of serial port to data, and return the length of it.
 * 
 * algorithm:
 *  the rx0BufferPosition point the last byte of RX Buffer.
 *  let the data follow the rx0BufferPosition's decrease to copy the data.
 *  like the following diagram:
 * 
 *  rx0BufferAddress       rx0BufferPosition
 *     |                     |
 *     V              <--    V
 *     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 *                     <--    ^
 *                            |
 *                           data
 */
int SPDI_ReadAll(portType port, char *data, uchar length)
{
    if (!SPDI_IsReadyToRead(port))
        return SP_ERR_READ_NOT_READY;

    switch (port) {
    case URAT0:
        if (length < (rx0BufferPosition - rx0BufferAddress))
            return SP_ERR_BUFFER_TOO_SHORT;
        
        data = rx0BufferPosition + 1;
        
        do {
            *(--data) = *rx0BufferPosition;
        }while(--rx0BufferPosition < rx0BufferAddress);
        break;
    case URAT1:
        if (length < (rx1BufferPosition - rx1BufferAddress))
            return SP_ERR_BUFFER_TOO_SHORT;
        
        data = rx1BufferPosition + 1;
        
        do {
            *(--data) = *rx1BufferPosition;
        }while(--rx1BufferPosition < rx1BufferAddress);
        break;
    default:
        return FALSE;
    }
    
    return SP_NORMAL;
}

/*
 * Name: SPDI_Write
 * Write the data with length to seril port and return the result.
 *
 * algorithm:
 *  the tx0BufferPosition point the TX0 Buffer address.
 *  the data point the last byte of data.
 *  this two points move towards difference direction.
 *  like the following diagram:
 * 
 *  tx0BufferPosition
 *     |
 *     V -->
 *     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 *     ^                        <-- ^
 *     |<-------- length ---------->|
 *  tx0BufferAddress               data
 * 
 *  The reason why we do this is to make the xxPosition pointer
 *  can signify the length of buffer with xxAddress.
 * 
 *  When sending the data, xxPosition will decrease. You can see
 *  that using the method above can insure that the data are sending
 *  in right order. 
 */
int SPDI_Write(portType port, char *data, uchar length)
{
    if (!SPDI_IsReadyToWrite(port))
        return SP_ERR_WRITE_NOT_READY;

    switch(port) {
    case URAT0:
        tx0BufferPosition = tx0BufferAddress;
        data += length - 1;
        while (tx0BufferPosition < tx0BufferAddress + length) {
             *(tx0BufferPosition++) = *(data--);
        }
        UC0IE |= UCA0TXIE;              // Open the transfer interrupt
        break;
    case URAT1:
        tx1BufferPosition = tx1BufferAddress;
        data += length - 1;
        while (tx1BufferPosition < tx1BufferAddress + length) {
             *(tx1BufferPosition++) = *(data--);
        }
        UC1IE |= UCA1TXIE;              // Open the transfer interrupt
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }
    
    return SP_NORMAL;
}



///////////////////////////////////////////////////////////////////////////
// Serial port interrupt functions

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    rx0FuncP(UCA0RXBUF);
}
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR(void)
{
    rx1FuncP(UCA1RXBUF);
}
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    UC0IE &= ~UCA0TXIE;             // Close the transfer interrupt first.
    // sending the reast content if not reach the head
    if (tx0BufferAddress != tx0BufferPosition) {
        UC0IE |= UCA0TXIE;          // Open the transfer interrupt
        UCA0TXBUF = *(--tx0BufferPosition);
    }
    tx0FuncP();
}
#pragma vector=USCIAB1TX_VECTOR
__interrupt void USCI1TX_ISR(void)
{
    UC1IE &= ~UCA1TXIE;             // Close the transfer interrupt
    // sending the reast content if not reach the head
    if (tx1BufferAddress != tx1BufferPosition) {
        UC1IE |= UCA1TXIE;          // Open the transfer interrupt
        UCA1TXBUF = *(--tx1BufferPosition);
    }
    tx1FuncP();
}
