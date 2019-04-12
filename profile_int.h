#ifndef __Profile_H__
#define __Profile_H__
#include "profile.h"
#include "vscode.h"
#pragma vector = PORT1_VECTOR
#define UCP_INIT 500
int16_t ucp = UCP_INIT;

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
uint16_t ADC_DC_Val_10 = 0;
uint32_t ADC_DC_Val = 0;
uint8_t ADC_NUM = 0;
#ifdef __ADC_SCH_H__
#pragma vector = ADC12_VECTOR
interrupt void ADC12_ISR()
{
#if ENABLE_VPP_DETECT
    ((ADC12MEM0 > V_max) && (V_max = ADC12MEM0)) || (ADC12MEM0 < V_min) && (V_min = ADC12MEM0);
#endif
#if ENABLE_VDC_DETECT
    ADC_DC_Val += ADC12MEM0;
    ADC_NUM = ADC_NUM + 1;
#endif
    ADC12CTL0 |= ADC12SC;
    ADC12IFG = 0X00;
}
#define MAX_SAMPLE_RES 10
int16_t Max_Sample = MAX_SAMPLE_RES;
int16_t proc = 400;
#endif // __ADC_SCH_H__
#pragma vector = PWM_TIMERx_A1_VECTOR
interrupt void PWM_A1_ISR()
{
#if ENABLE_VDC_DETECT
    ADC_DC_Val_10 += ADC_DC_Val;
    if (!--Max_Sample)
    {
        __disable_interrupt();
        ADC_DC_Val = ADC_DC_Val_10 / ADC_NUM;
        proc += IncPIDCalc(ADC_DC_Val);
        if(proc>800)proc=800;
        if(proc<0)proc=0;
        //SetPWM2(proc);
        ADC_DC_Val_10 = 0;
        Max_Sample = MAX_SAMPLE_RES;
        ADC_NUM = 0;
        TA2CTL &= ~TAIFG;
        __enable_interrupt();
    }
    ADC_DC_Val = 0;
#endif
}
#endif // !_PROFILE_H_
