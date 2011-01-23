#ifndef __KEYBOARD_DRIVER_H__
#define __KEYBOARD_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#include "helper_bits.h"

#define unsigned char byte
#define byte uchar

/* define the key value */
#define KEY_NULL   0
#define KEY_NUM1   1
#define KEY_NUM2   2
#define KEY_NUM3   3
#define KEY_NUM4   5
#define KEY_NUM5   6
#define KEY_NUM6   7
#define KEY_NUM7   9
#define KEY_NUM8   10
#define KEY_NUM9   11
#define KEY_NUMX   13
#define KEY_NUM0   14
#define KEY_NUMJ   15
#define KEY_OK     4
#define KEY_BACK   8
#define KEY_DIAL   12
#define KEY_HANGUP 16


typedef enum EnumKeyEventKind
{
    SingleClick,   
    DoubleClick,
    LongPress,
    Nothing
}EKeyEventKind;

typedef struct StructKeyEvent 
{
    EKeyEventKind eEventKind;
    unsigned char ucKeyNum;
}SKeyEvent;

void KDI_Init(void);
void KDI_Scan(void);
void KDI_GetCurrentKey(uchar *CurrentKey, uchar *KeyEvent);


#ifdef __cplusplus
}
#endif

#endif
