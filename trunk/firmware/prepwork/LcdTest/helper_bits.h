#ifndef HELPER_BITS_H_
#define HELPER_BITS_H_


#define BIT8_0		(0x01)
#define BIT8_1		(0x02)
#define BIT8_2		(0x04)
#define BIT8_3		(0x08)
#define BIT8_4		(0x10)
#define BIT8_5		(0x20)
#define BIT8_6		(0x40)
#define BIT8_7		(0x80)

#define BIT_SET(x, y) (x |= y)
#define BIT_UNSET(x, y) (x &= ~y)


#endif /*HELPER_BITS_H_*/
