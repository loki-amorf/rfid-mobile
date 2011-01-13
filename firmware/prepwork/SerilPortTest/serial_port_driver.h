/*
 * serial_port_driver.h
 */
#ifndef __SERIAL_PORT_DRIVER_H_
#define __SERIAL_PORT_DRIVER_H_

#define URAT0   0x30                    // P3.4,5 = USCI_A0 TXD/RXD
#define URAT1   0xC0                    // P3.6,7 = USCI_A1 TXD/RXD
#define URATALL (URAT0 | URAT1)

#define BAUDRATE115k 0
#define BAUDRATE9600 1

int SPDI_Initial(int port3, char br);

#endif /* __SERIAL_PORT_DRIVER_H_ */
