#ifndef __Profile_H__
#define __Profile_H__
#include "profile.h"
#include "vscode.h"
#pragma vector = PORT1_VECTOR
interrupt void P1_ISR()
{
    uint8_t _P1IFG = P1IFG;
    switch (__even_in_range(P1IV, 0x10))
    {
#if EnableButtonMartix
    case P1IV_P1IFG2:
    case P1IV_P1IFG3:
    case P1IV_P1IFG4:
    case P1IV_P1IFG5:
        Button_Martix_ISR(_P1IFG);
#endif
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A1_BASE
    case P1IV_P1IFG6:
        REAL_GATE_ISR();
        /* code */
        break;
#endif
    default:
        break;
    }
    P1IFG = 0X00;
}
#pragma vector = PORT2_VECTOR
interrupt void P2_ISR()
{

    switch (__even_in_range(P2IV, 0x10))
    {
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    case P2IV_P2IFG2:
        /* code */
        TA2CTL;
        REAL_GATE_ISR();
        break;
#endif
    default:
        break;
    }
    P2IFG = 0X00;
}
#ifdef __ADC_SCH_H__
#pragma vector = ADC12_VECTOR
interrupt void ADC12_ISR()
{
    ADC12_SRV();
    ADC12IFG = 0X00;
}
#endif // __ADC_SCH_H__
#if defined(PWM_H) && defined(BUCK_H)
uint8_t sec = 50;
#pragma vector = PWM_TIMERx_A1_VECTOR
interrupt void PWM_A1_ISR()
{
    if (ADC12_SCH_MEM > (BUCK_INV + BUCK_INVC))
    {
        SetPWM2Persent(--sec);
    }
    else if (ADC12_SCH_MEM < (BUCK_INV - BUCK_INVC))
    {
        SetPWM2Persent(++sec);
    }
    TA2CTL &=~ TAIFG;
}
#endif
#endif // !_PROFILE_H_
