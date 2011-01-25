/**
 * serial_port_driver.c
 */
#include "serial_port_driver.h"

// define the buffer length of serial port.
#define BUF_LENGTH  40

// define overtime counts
#define SP_OVERTIME_COUNT   10000

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
// define recieve exchange buffer variables.
static char rx1BufferAddress[BUF_LENGTH];
static char *rx1BufferPosition;

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
        // if not ready, return FALSE
        if (!(UC0IFG&UCA0TXIFG)) {
            return FALSE;
        }
        break;
    case URAT1:
        // if not ready, return FALSE
        if (!(UC1IFG&UCA1TXIFG)) {
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
        if (rx0BufferPosition == rx0BufferAddress)
            return FALSE;   // if not ready, return FALSE
        break;
    case URAT1:
        if (rx1BufferPosition == rx1BufferAddress)
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
 * Note:
 *  at first I want to use the algorithm follow:
 *  the rx0BufferPosition point the last byte of RX Buffer.
 *  let the data follow the rx0BufferPosition's decrease to copy the data.
 *  like the following diagram:
 * 
 *  rx0BufferAddress       rx0BufferPosition
 *     |                     |
 *     V              <--    V
 *     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 *     xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
 *                    <--    ^
 *                           |
 *                          data
 * 
 * But, the interrupt messed the step of data exchange, so, using the index
 * and judge every loop is more stable I think.
 *     -- tankery.chen@gmail.com
 */
int SPDI_ReadAll(portType port, char *data, uchar length)
{
    int index;
    if (!SPDI_IsReadyToRead(port))
        return SP_ERR_READ_NOT_READY;

    switch (port) {
    case URAT0:
        for (index = 0; index < (rx0BufferPosition - rx0BufferAddress); ++index) {
            // make sure the length of data is enough;
            if (length < index+1)
                return SP_ERR_BUFFER_TOO_SHORT;
            // exchange the data
            *(data + index) = *(rx0BufferAddress + index);
        }
        // exchange finished, reset position.
        rx0BufferPosition = rx0BufferAddress;
        UC0IE |= UCA0RXIE;  // make sure the RXI is enable
        break;
    case URAT1:
        for (index = 0; index < (rx1BufferPosition - rx1BufferAddress); ++index) {
            // make sure the length of data is enough;
            if (length < index+1)
                return SP_ERR_BUFFER_TOO_SHORT;
            // exchange the data
            *(data + index) = *(rx1BufferAddress + index);
        }
        // exchange finished, reset position.
        rx1BufferPosition = rx1BufferAddress;
        UC1IE |= UCA1RXIE;  // make sure the RXI is enable
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }

    return index;
}

/*
 * Name: SPDI_Write
 *  Write the data with length to seril port and return the result.
 */
int SPDI_Write(portType port, char *data, uchar length)
{
    char *byte;
    int  tc= 0;        // for overtime count

    if (!SPDI_IsReadyToWrite(port))
        return SP_ERR_WRITE_NOT_READY;

    switch(port) {
    case URAT0:
        // serial write the data
        for (byte = data; byte < data + length; ++byte) {
            // wait transfer buffer ready till overtime
            while (!(UC0IFG&UCA0TXIFG)) {
                if (++tc == SP_OVERTIME_COUNT)
                    return SP_ERR_WRITE_OVERTIME;
            }

            UCA0TXBUF = *byte;
        }
        break;
    case URAT1:
        // serial write the data
        for (byte = data; byte < data + length; ++byte) {
            // wait transfer buffer ready till overtime
            while (!(UC1IFG&UCA1TXIFG)) {
                if (++tc == SP_OVERTIME_COUNT)
                    return SP_ERR_WRITE_OVERTIME;
            }

            UCA1TXBUF = *byte;
        }
        break;
    default:
        return SP_ERR_NO_SERIL_PORT;
    }
    
    return SP_NORMAL;
}



///////////////////////////////////////////////////////////////////////////
// Serial port interrupt functions

// URAT0 recive exchange interrupt.
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if (rx0BufferPosition < rx0BufferAddress + BUF_LENGTH) {
        *rx0BufferPosition = UCA0RXBUF;
        ++rx0BufferPosition;
    }
    else
        UC0IE &= ~UCA0RXIE; // disable interrupt when buffer is full.
    rx0FuncP(UCA0RXBUF);
}
// URAT1 recive exchange interrupt.
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR(void)
{
    if (rx1BufferPosition < rx1BufferAddress + BUF_LENGTH) {
        *(rx1BufferPosition) = UCA1RXBUF;
        ++rx1BufferPosition;
    }
    else
        UC1IE &= ~UCA1RXIE; // disable interrupt when buffer is full.
    rx1FuncP(UCA1RXBUF);
}
// URAT0 transfer exchange interrupt.
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    tx0FuncP();
}
// URAT1 transfer exchange interrupt.
#pragma vector=USCIAB1TX_VECTOR
__interrupt void USCI1TX_ISR(void)
{
    tx1FuncP();
}
