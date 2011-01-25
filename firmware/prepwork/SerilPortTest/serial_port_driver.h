/**
 * serial_port_driver.h
 *
 *     目前来说，部分函数（SPDI_IsReadyToWrite等）在中断中的处理不是很好，
 * 尽量不要在中断中调用这些函数（当然，需要的话可以尝试）。最好如示例一
 * 样，在主流程中探测并执行。
 *
 *     若要使用中断，先声明一个函数，这个函数是希望其在中断到来后被调用
 * 的函数。然后使用SPDI_SetRX0FunP系列函数指定中断执行该函数。（使用函数
 * 指针实现。可参考实例）
 *
 *     -- tankery.chen@gmail.com				Jan 25, 2011
 */
#ifndef __SERIAL_PORT_DRIVER_H_
#define __SERIAL_PORT_DRIVER_H_
#include "msp430x24x.h"

#define URAT0           0x30                // P3.4,5 = USCI_A0 TXD/RXD
#define URAT1           0xC0                // P3.6,7 = USCI_A1 TXD/RXD

#define BAUDRATE115k    0
#define BAUDRATE9600    1

#define SP_NORMAL               0           // serial port operation is normal.
#define SP_ERR_CAL_CONST_ERASED -1           // calibration constants erased
#define SP_ERR_NO_BAUDRATE      -2           // no such type of baudrate.
#define SP_ERR_NO_SERIL_PORT    -3           // no such type of seril port.
#define SP_ERR_BUFFER_TOO_SHORT -4           // the input data buffer too short.
#define SP_ERR_WRITE_NOT_READY  -5           // not ready to write
#define SP_ERR_READ_NOT_READY   -6           // not ready to read
#define SP_ERR_WRITE_OVERTIME   -7           // serial write overtime.

#define TRUE    1
#define FALSE   0

typedef unsigned char uchar;
typedef uchar portType;

/* define Recive eXchange Interrupt Function Point and
 * transfer the recive buffer's data to function's input. */
typedef void (* RXIFunP)(char);
/* define Transfer eXchange Interrupt Function Point.
 * if transfer buffer is ready, the function of this type
 * of point will be calling. */
typedef void (* TXIFunP)(void);

/* This function must be calling first to initialize the environment and
 * specified the buffer address.
 * Return none zero if the initialization failed. */
int SPDI_Initial();

/* This four function assign the function points to the given function,
 * so the given function will be callback when interrupts occur. */
void SPDI_SetRX0FunP(RXIFunP fp);
void SPDI_SetRX1FunP(RXIFunP fp);
void SPDI_SetTX0FunP(TXIFunP fp);
void SPDI_SetTX1FunP(TXIFunP fp);

int SPDI_SetBaudRate(portType port, int br);

/* Open the serial port specified by var 'port3'. Note this function's
 * has appendding effect. That is to say, you can recalling this function
 * to reopen or modify a serial port. */
int SPDI_Open(portType port, int br);
int SPDI_Close(portType port);

/* Return TRUE if the UCAxTXBUF is empty */
int SPDI_IsReadyToWrite(portType port);
/* Return TRUE if the UCAxRXBUF has received a complete character */
int SPDI_IsReadyToRead(portType port);

/* Read the buffer of serial port to data, and return the result. */
int SPDI_ReadAll(portType port, char *data, uchar length);
/* Write the data with length to seril port and return the result. */
int SPDI_Write(portType port, char *data, uchar length);


#endif /* __SERIAL_PORT_DRIVER_H_ */
