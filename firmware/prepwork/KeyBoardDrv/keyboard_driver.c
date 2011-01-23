#include "msp430x24x.h"
#include "helper_bits.h"
#include "keyboard_driver.h"


/* Global variable declaration */
bit bOneClick;
bit bLongPress;
bit bDoubleClick;
bit bIsInDoubleClick;
bit bAllowLongPress;

byte ucBBtn;           /* possible clicked button before */
byte ucCBtn;           /* current possible clicked button */
byte ucLastPress;      /* last certain clicked button */
byte ucHighTime;
byte ucLowTime;
byte ucHighTimeBefore;

static int CheckState(void);
static void JudgeNoChange(void);
static void JudgeChange(void);
static SKeyEvent GetKeyState(void);


/***************************************************************
Function:KDI_Init()
Discription:This function is used to initialize the vars used 
            in getting the status of the keyboard
***************************************************************/
void KDI_Init(void)
{
    ucBBtn = 0;
    ucCBtn = 0;
    ucLastPress = 0;  
    ucHighTime = 0;
    ucLowTime = 0;

    bOneClick = 0;
    bLongPress = 0;
    bDoubleClick = 0;
    bIsInDoubleClick = 0;
    bAllowLongPress = 0;

    return;
}

/***************************************************************
Function:KDI_Scan()
Discription:Every time we get into the function,
            we check the keyboard status
***************************************************************/
void KDI_Scan(void)
{
    CheckState();
    if(ucBBtn == ucCBtn)
    {
        JudgeNoChange();
    }
    else
    {
        JudgeChange();
    }
    
    return;
}

/***************************************************************
Function:KDI_GetCurrentKey()
Discription:The fuction is the main interface of the lib
***************************************************************/
void KDI_GetCurrentKey(uchar *CurrentKey, EKeyEventKind *KeyEvent)
{
    SKeyEvent KeyInfo;

    KeyInfo = GetKeyState();

    *CurrentKey = KeyInfo.ucKeyNum;
    *KeyEvent = KeyInfo.eEventKind;

    return;    
}

/***************************************************************
Function:   CheckState()
Discription:This function is used to check the button pressed 
Output:     The number of the pressed button ( 1 - 16 )
            return 0 means nothing pressed
key value:
       col£º[p14]  [p15]  [p16]   [p17]
             ¡ý     ¡ý     ¡ý     ¡ý
  row£º
[p10]¡ú      1      2      3      4   
[p11]¡ú      5      6      7      8    
[p12]¡ú      9     10     11     12   
[p13]¡ú     13     14     15     16   

***************************************************************/
static int CheckState(void)
{
    uchar cord_h;  // high 4 bits, column value
    uchar cord_l;  // low 4 bits, row value
    uchar cord;
    uchar key_value = KEY_NULL;
    uchar i = 16;
    /* the keyboard code value mapping to key value, 1-16 */
    uchar key_mapping[16] = {0xee, 0xde, 0xbe, 0x7e,
                             0xed, 0xdd, 0xbd, 0x7d,
                             0xeb, 0xdb, 0xbb, 0x7b,
                             0xe7, 0xd7, 0xb7, 0x77};
    
    /* set the button pressed before */
    ucBBtn = ucCBtn;

    P1IN = 0x0f;            // set column value to 0
    cord_l = P1&0x0f;     // get the row value

    if (cord_l == 0x0f)   // there is no key pressed
    {
        return KEY_NULL;
    }
    
    P1OUT = cord_l|0xf0;     // get current P1
    cord_h = P1OUT&0xf0;     // get the column value
    cord = cord_h+cord_l; // the whole code value of keyboard
    
    do{
        if (cord = key_mapping[i-1])
        {
            key_value = i;    // get the pressed key value
            break;
        }
        
    }while((--i) != 0)
    
    return key_value;
}

/***************************************************************
Function:JudgeNoChange()
Discription:Every time we find that the former key and the 
            current key are the same, we get in this function.
***************************************************************/
static void JudgeNoChange(void)
{
    //relax
    if(ucCBtn == KEY_NULL)
    {        
        if(ucHighTime == 0)
        {            
            if(ucLowTime == 0)
            {
                ucHighTimeBefore = ucHighTime;
                return;
            }
            if(bIsInDoubleClick)
            {
                ucHighTimeBefore = ucHighTime;
                return;
            }
            if(ucHighTimeBefore == 1)
            {
                bOneClick = 1;
                ucHighTimeBefore = ucHighTime;
            }
        }
        else
        {
            ucHighTimeBefore = ucHighTime;
            ucHighTime--;
        }
    }
    //hold
    else
    {
        if(ucLowTime == 0)
        {
            bLongPress = 1;
        }
        else
        {
            ucLowTime--;            
        }
    }   

    return;
}


/***************************************************************
Function:JudgeChange()
Discription:Every time we find that the former key and the 
            current key are the same, we get in this function.
***************************************************************/
static void JudgeChange(void)
{
    bit bTempInDoubleClick;
    if(ucCBtn == KEY_NULL)
    {
        ucLastPress = ucBBtn;
        ucHighTime = 10;
    }
    else
    {
        bTempInDoubleClick = bIsInDoubleClick;
        bIsInDoubleClick = 0;
        bAllowLongPress = 1;
        if(ucLowTime == 0)
        {
            ucLowTime = 100;
            return;
        }
        else
        {
             ucLowTime = 100;
        }
        if(ucHighTime == 0)
        {
            return;
        }
        if(ucCBtn == ucLastPress)
        {
            if(bTempInDoubleClick == 1)
            {
                return;
            }
            bIsInDoubleClick = 1;
            bDoubleClick = 1;
        }
        else
        {
            bOneClick = 1;
        }
    }

    return;
}

/***************************************************************
Function:GetKeyState()
Discription:The fuction is the main interface of the lib
***************************************************************/
static SKeyEvent GetKeyState(void)
{
    SKeyEvent returnValue;
    if(bOneClick == 1)
    {
        bOneClick = 0;
        returnValue.eEventKind = SingleClick;        
        returnValue.ucKeyNum = ucLastPress;
    }
    else if(bDoubleClick == 1)
    {
        bDoubleClick = 0;
        returnValue.eEventKind = DoubleClick;
        returnValue.ucKeyNum = ucLastPress;
    }
    else if(bLongPress == 1)
    {        
        bLongPress = 0;
        if(!bAllowLongPress)
        {
            returnValue.eEventKind = Nothing;
            return returnValue;
        }
        returnValue.eEventKind = LongPress;  
        returnValue.ucKeyNum = ucCBtn;
        bAllowLongPress = 0;
    }
    else
    {
        returnValue.eEventKind = Nothing;
        returnValue.ucKeyNum = 0xff;
    }

    //if (returnValue.ucKeyNum == 10)     // make key 10 to key 0
    //    returnValue.ucKeyNum= 0;

    return returnValue;
}


