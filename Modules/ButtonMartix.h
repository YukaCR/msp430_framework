
#ifndef BUTTON_MARTIX_H
#define BUTTON_MARTIX_H
#include "../vscode.h"
#include <msp430.h>
/*薄膜键盘按键值对应:
 * 有字的一面对着芯片插入
 * '*':0   '0':1  '#':2  'D':3
 * '7':4   '8':5  '9':6  'C':7
 * '4':8   '5':9  '6':10 'B':11
 * '1':12  '2':13 '3':14 'A':15
*/

uint8_t KeyValue = 0;
char KeyCode = 0;
char KeyTable[]={
    '*','0','#','D',
    '7','8','9','C',
    '4','5','6','B',
    '1','2','3','A'
};
bool Button_Flag = 0;
bool Button_LT_Flag = 0;
void Setup_Button_Martix()
{
    P4DIR |= (BIT3 + BIT0);
    P3DIR |= BIT7;
    P8DIR |= BIT2;
    P1OUT |= (BIT2 + BIT3 + BIT4 + BIT5); //Set High
    P4OUT &= ~(BIT3 + BIT0);
    P3OUT &= ~BIT7;
    P8OUT &= ~BIT2;
    P1IES |= (BIT2 + BIT3 + BIT4 + BIT5);
    P1REN |= (BIT2 + BIT3 + BIT4 + BIT5); //enable pullup
    P1IE |= (BIT2 + BIT3 + BIT4 + BIT5);
}
void Button_Martix_ISR(uint8_t _P1IFG)
{
    uint8_t result;
    __disable_interrupt();
    __delay_cycles(24 * 5); //5us
    if (Button_LT_Flag)
    {
        Button_LT_Flag = 0;
        P1IES |= (BIT2 + BIT3 + BIT4 + BIT5);
        P1IFG = 0X00;
        __enable_interrupt();
        UCA0IFG &= ~UCTXIFG;
        UCA0IFG |= UCTXIFG;
        return;
    }
    do
    {
        if (!(P1IN & _P1IFG))
        {
            KeyValue = _P1IFG << 2;
            P4OUT |= BIT3;
            if (P1IN & _P1IFG)
            {
                KeyValue |= BIT0;
                break;
            }
            P4OUT |= BIT0;
            if (P1IN & _P1IFG)
            {
                KeyValue |= BIT1;
                break;
            }
            P3OUT |= BIT7;
            if (P1IN & _P1IFG)
            {
                KeyValue |= BIT2;
                break;
            }
            P8OUT |= BIT2;
            if (P1IN & _P1IFG)
            {
                KeyValue |= BIT3;
                break;
            }
        }
        else
        {
            return;
        }
    } while (0);
    // raw data to readable data
    if(KeyValue & 0x80){
        result = 3;
    }
    else{
        result = (KeyValue >> 5 )% 5;
    }
    result *= 4;
    if(KeyValue & 0x08){
        result += 3;
    }
    else{
        result += ((KeyValue&0x0f)>>1 )% 5;
    };
    KeyValue = result;
    KeyCode = KeyTable[KeyValue];
    Button_Flag = 1;
    Button_LT_Flag = 1;
    P4OUT &= ~(BIT3 + BIT0);
    P3OUT &= ~BIT7;
    P8OUT &= ~BIT2;
    P1IES &= ~(BIT2 + BIT3 + BIT4 + BIT5);
    P1IFG = 0X00;
    __enable_interrupt();
    UCA0IFG &= ~UCTXIFG;
    UCA0IFG |= UCTXIFG;
}
#pragma vector = PORT1_VECTOR
interrupt void P1_ISR()
{
    uint8_t _P1IFG = P1IFG;
    switch (__even_in_range(P1IV, 0x10))
    {
    case P1IV_P1IFG2:
    case P1IV_P1IFG3:
    case P1IV_P1IFG4:
    case P1IV_P1IFG5:
        Button_Martix_ISR(_P1IFG);
    default:
        break;
    }
    P1IFG = 0X00;
}
#endif
