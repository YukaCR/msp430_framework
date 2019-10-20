/*
*   Author: YukaCR
*   aka DigitalPowerModules/dpm_pwmc_33Mhz_40kHz.h
*/
#ifdef PWM_RR_C
#warning "PWM_Burn.h don't include another PWM.H if something wrong(x"
#endif
#ifndef PWM_H
#define PWM_H
#define PWM_BR_C
#include <msp430.h>
#include "vscode.h"
#include "../TIDriver/timer_a.h"
#include "../TIDriver/ucs.h"
#include "../profile.h"
#include "MPY32.h"
#define TIMER_A2_INUSE
/*
    Using TimerA2,PWM at 40KHz , using 33MHz DCO.
    F5529   
    P2.2 <- Oscillator
    P2.5 -> PWM1 out
    P2.4 -> PWM2 out
*/
#define Mhz 1000000L
#define Peroid 550
#define Peroid_10 5
void InitPWM(uint16_t PWM1_Value = 0, uint16_t PWM2_Value = 0)
{
    //Fucking 5529 has a 4MHz Osc
    //Using Pierce Oscillator, 12MHz PWM_Opt.clockSource = TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK;
    //Increase accuracy using 12MHz External Oscillator
    //Result At 12MHz, PWM_Opt.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    //to 10KHz , using TACCR , 12 * MHz / 10 * 2 * KHz = 600(?????Why so low)
    //Use as comprator PWM_Opt.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    P2SEL |= BIT2 | BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT5 | BIT4;
    P5SEL |= BIT4 | BIT5 | BIT2 | BIT3;
    //burnDCO();
    UCS_initClockSignal(UCS_ACLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    PWM_TAxCTL = TASSEL__ACLK + ID_0 + MC__STOP;
    PWM_TAxCCR0 = Peroid;

#warning "PWM_Burn TimerA2CCR0 Interrupt disabled"
    PWM_TAxCCTL0 = CM_1 + SCS + SCCI ;    //TA0.0捕获
    PWM_TAxCCTL2 = CM_0 + SCS + PWM1MODE + OUT;
    PWM_TAxCCTL1 = CM_0 + SCS + PWM2MODE + OUT;
    PWM_TAxCCR1 = PWM2_Value;
    PWM_TAxCCR2 = PWM1_Value;
    PWM_TAxCTL |= MC__UP + TACLR;
    __enable_interrupt();
}

inline void InitPWMPercent(uint16_t PWM1Value = 0, uint16_t PWM2Value = 0)
{
    return InitPWM(PWM1Value * Peroid_10, PWM2Value * Peroid_10);
}
inline void SetPWM1(uint16_t Value)
{
    PWM_TAxCCR2 = Value;
}
inline void SetPWM2(uint16_t Value)
{
    
    PWM_TAxCCR1 = Value;
}
inline void SetPWM1Persent(uint16_t Value)
{
    HW_MPY_16_16_STA_U(Value,Peroid_10,PWM_TAxCCR2);
}
inline void SetPWM2Persent(uint16_t Value)
{
    HW_MPY_16_16_STA_U(Value,Peroid_10,PWM_TAxCCR1);
}
#endif
