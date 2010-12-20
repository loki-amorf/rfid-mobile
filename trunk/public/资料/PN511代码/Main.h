#ifndef __MAIN__H__
#define __MAIN__H_

#define OSC_FREQ                11059200L//18432000L

#define BAUD_115200             256 - (OSC_FREQ/192L)/115200L   // 255
#define BAUD_57600              256 - (OSC_FREQ/192L)/57600L    // 254
#define BAUD_38400              256 - (OSC_FREQ/192L)/38400L    // 253
#define BAUD_28800              256 - (OSC_FREQ/192L)/28800L    // 252
#define BAUD_19200              256 - (OSC_FREQ/192L)/19200L    // 250
#define BAUD_14400              256 - (OSC_FREQ/192L)/14400L    // 248
#define BAUD_9600               256 - (OSC_FREQ/192L)/9600L     // 244
#define RCAP2_50us              65536L - OSC_FREQ/240417L
#define RCAP2_1ms               65536L - OSC_FREQ/12021L
#define RCAP2_10ms              65536L - OSC_FREQ/1200L
#define RCAP2_1s                65536L - OSC_FREQ/12L

#endif

