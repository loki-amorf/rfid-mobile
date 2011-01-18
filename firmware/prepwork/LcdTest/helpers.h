#ifndef HELPER_TYPES_H_
#define HELPER_TYPES_H_


#include "helper_bits.h"

typedef unsigned char uint8;

void delay(int x)
{
    int i, j;
    
    i = x;
    do {
        j = 100;
        do {
            --j;
        } while (j != 0);
    } while (i != 0);
}

#endif /*HELPER_TYPES_H_*/
