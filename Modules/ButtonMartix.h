
#ifndef BUTTON_MARTIX_H
#define BUTTON_MARTIX_H
#include "vscode.h"
#include "profile.h"
#include <msp430.h>
uint8_t KeyValue = 0;
void (*ButtonCallback)(uint8_t) = nullptr;
void Button_Martix_Init()
{
    P4DIR |= BIT3 + BIT0;
    P3DIR |= BIT7;
    P8DIR |= BIT2;
    P1OUT |= BIT2 + BIT3 + BIT4 + BIT5; //Set High
    P4OUT &=~ BIT3 + BIT0;
    P3OUT &=~ BIT7;
    P8OUT &=~ BIT2;
    P1IES |= BIT2 + BIT3 + BIT4 + BIT5;
    P1REN |= BIT2 + BIT3 + BIT4 + BIT5; //enable pullup
    P1IE |= BIT2 + BIT3 + BIT4 + BIT5;
}
inline void Button_Martix_ISR(uint8_t _P1IFG,uint8_t _P1IV)
{
    __delay_cycles(24 * 5); //5us
    do
    {
        if (!(P1IN & _P1IFG))
        {
            KeyValue = _P1IFG << 2;
            P1IE &= ~BIT2 + BIT3 + BIT4 + BIT5;
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
    if(ButtonCallback != nullptr){
        ButtonCallback(KeyValue);
    }
    P4OUT &=~ BIT3 + BIT0;
    P3OUT &=~ BIT7;
    P8OUT &=~ BIT2;
    P1IE |= BIT2 + BIT3 + BIT4 + BIT5;
}
#endif

