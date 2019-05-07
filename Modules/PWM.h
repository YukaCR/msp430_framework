#ifndef PWM_H
#define PWM_H
#define PWM_RR_C
#ifdef PWM_BR_C
#warning "PWM_Burn.h don't include another PWM.H if something wrong(x"
#endif
#include <msp430.h>
#include "../TIDriver/timer_a.h"
#include "../TIDriver/ucs.h"
#include "../profile.h"
/*
    Using TimerA2,PWM at 500kHz
    F5529   
    P2.2 <- Oscillator
    P2.5 -> PWM1 out
    P2.4 -> PWM2 out
*/
#define Mhz 1000000L
//Calc <- SPWM_Calculator.py
#define Period 400
#define SPWM_TOTAL 200 //Method: TriangleWaveFreq / SinWaveFreq
#if EnableSPWM
const uint16_t SPWM_Period[400] = {272, 277, 280, 285, 289, 293, 298, 302, 306, 310, 314, 318, 323, 327, 331, 335, 339, 343, 348, 351, 356, 359, 364, 367, 372, 375, 380, 383, 387, 390, 395, 398, 403, 405, 410, 412, 417, 419, 424, 426, 431, 433, 438, 440, 445, 446, 451, 452, 457, 459, 463, 465, 469, 470, 475, 476, 480, 481, 485, 486,
                                   490, 491, 495, 496, 500, 501, 504, 505, 508, 509, 512, 513, 516, 516, 519, 520, 522, 523, 525, 526, 528, 528, 530, 531, 532, 533, 534, 534, 536, 536, 537, 537, 538, 538, 539, 539, 540, 540, 540, 540, 540, 540, 540, 540, 539, 539, 538, 538, 537, 537, 536, 536, 534, 534, 533, 532, 531, 530, 528, 528, 526, 525, 523, 522, 520, 519, 516, 516, 513, 512, 509, 508, 505, 504, 501, 500, 496, 495, 491, 490, 486, 485, 481, 480, 476, 475, 470, 469, 465, 463, 459, 457, 452, 451, 446, 445, 440, 438, 433, 431, 426, 424, 419, 417, 412, 410, 405, 403, 398, 395, 390, 387, 383, 380, 375, 372, 367, 364, 359, 356, 351, 348, 343, 339, 335, 331, 327, 323, 318, 314, 310, 306, 302, 298, 293, 289, 285, 280, 277, 272, 268, 263, 260, 255, 251, 246, 243, 238, 235, 229, 226, 221, 218, 213, 210, 204, 201, 196, 193, 188, 185, 180, 177, 172, 169, 164,
                                   162, 156, 154, 148, 147, 141, 139, 133, 132, 126, 125, 119, 118, 112, 111, 105, 104, 98, 97, 92, 91, 86, 85, 79, 79, 74, 73, 68, 67, 62, 62, 57, 56, 52, 51, 47, 47, 42, 42, 38, 37, 34, 33, 30, 29, 26, 26, 22, 22, 19, 19, 16, 16, 13, 13, 11, 11, 8, 8, 7, 6, 5, 5, 3, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 5,
                                   5, 6, 7, 8, 8, 11, 11, 13, 13, 16, 16, 19, 19, 22, 22, 26, 26, 29, 30, 33, 34, 37, 38, 42, 42, 47, 47, 51, 52, 56, 57, 62, 62, 67, 68, 73, 74, 79, 79, 85, 86, 91, 92, 97, 98, 104, 105, 111, 112, 118, 119, 125, 126, 132, 133, 139, 141, 147, 148, 154, 156, 162, 164, 169, 172, 177, 180, 185, 188, 193, 196, 201, 204, 210, 213, 218, 221, 226, 229, 235, 238, 243, 246, 251, 255, 260, 263, 268};
uint16_t SPWM_Current = 0;
#else
#undef Period
#if Use4MHzCrystal
#if PWMFreq < 65
#define Period 65535 //too low
#else
uint16_t Period = (uint16_t)((double)4000000L / (double)PWMFreq) + 1;
#endif
#else
#if PWMFreq < 195
#define Period 65535 //too low
#else
uint16_t Period = (uint16_t)((double)12000000L / (double)PWMFreq);
#endif
#endif
#endif
#ifdef __MSP430F5529__
void InitPWM(uint16_t PWM1_Value = 0, uint16_t PWM2_Value = 0)
{
    //Fucking 5529 has a 4MHz Osc
    //Using Pierce Oscillator, 12MHz PWM_Opt.clockSource = TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK;
    //Increase accuracy using 12MHz External Oscillator
    //Result At 12MHz, PWM_Opt.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    //to 10KHz , using TACCR , 12 * MHz / 10 * 2 * KHz = 600(?????Why so low)
    //Use as comprator PWM_Opt.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
#if PWN_TAxBASE == TIMER_A2_BASE
    P2SEL |= BIT2 | BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT3 + BIT4;
    P5SEL |= BIT4 | BIT5 | BIT2 | BIT3;
#endif
#if Use4MHzCrystal
    UCS_setExternalClockSource(32768, 4 * Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_3);
    UCS_setExternalClockSource(32768, 4 * Mhz);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    PWM_TAxCTL = TASSEL__SMCLK + ID_0 + MC__STOP;
#else
    PWM_TAxCTL = TASSEL__TACLK + ID_0 + MC__STOP;
#endif
    PWM_TAxCCR0 = Period;
    PWM_TAxCCTL0 = CM_1 + SCS + SCCI + CAP + CCIE;
    PWM_TAxCCTL2 = CM_0 + SCS + PWM1MODE;
#if EnablePWM2
    PWM_TAxCCTL1 = CM_0 + SCS + PWM2MODE + OUT;
    PWM_TAxCCR1 = PWM2_Value;
#endif
    PWM_TAxCCR2 = PWM1_Value;
#if EnableSPWM
    PWM_TAxCTL |= MC__UPDOWN + TAIE + TACLR;
#elif ENABLE_BUCK
    PWM_TAxCTL |= MC__UP + TAIE + TACLR;
#else
    PWM_TAxCTL |= MC__UP + TACLR;
#endif
}
inline void InitPWMPercent(uint16_t PWM1Value = 0, uint16_t PWM2Value = 0)
{
#if EnablePercentOFF
    return InitPWM((uint16_t)((PWM1Value + PWM_PercentOFF) * (double)Period / 100.0), (uint16_t)((PWM2Value+PWM_PercentOFF) * (double)Period / 100.0));
    #else
    return InitPWM((uint16_t)(PWM1Value * (double)Period / 100.0), (uint16_t)(PWM2Value * (double)Period / 100.0));
    #endif
}
inline void SetPWM1(uint16_t Value)
{
    PWM_TAxCCR2 = Value;
}
void SetPWMFreq(uint32_t _PWMFreq)
{
    double PWM1_PRECENT = (double)PWM_TAxCCR2 / (double)PWM_TAxCCR0, PWM2_PRECENT = (double)PWM_TAxCCR1 / (double)PWM_TAxCCR0;
#if Use4MHzCrystal
    Period = 4 * MHz / _PWMFreq;
#else
    Period = 12 * MHz / _PWMFreq;
#endif
    PWM_TAxCTL &= ~MC_0;
    PWM_TAxCCR0 = Period;
    PWM_TAxCCR1 = PWM2_PRECENT * Period;
    PWM_TAxCCR2 = PWM1_PRECENT * Period;
    PWM_TAxCTL |= MC__UP;
};
#if EnablePWM2
inline void SetPWM2(uint16_t Value)
{
    PWM_TAxCCR1 = Value;
}
#endif
inline void SetPWM1Persent(uint16_t Value)
{
#if EnablePercentOFF
    PWM_TAxCCR2 = (uint16_t)((Value + PWM_PercentOFF) * (double)Period / 100.0);
#else
    PWM_TAxCCR2 = (uint16_t)((Value) * (double)Period / 100.0);
#endif
}
#if EnablePWM2
inline void SetPWM2Persent(uint16_t Value)
{
#if EnablePercentOFF
    PWM_TAxCCR1 = (uint16_t)((Value + PWM_PercentOFF) * (double)Period / 100.0);
#else
    PWM_TAxCCR1 = (uint16_t)((Value) * (double)Period / 100.0);
#endif
}
#endif
#pragma vector = TIMER2_A0_VECTOR
interrupt void PWM_ISR()
{
#if EnableSPWM
    (SPWM_TOTAL - SPWM_Current) && (SPWM_Current = 0);
    SetPWM(*(SPWM_Period + SPWM_Current++));
#endif
    PWM_TAxCTL &= ~TAIFG;
    PWM_TAxCCTL0 &= ~CCIFG;
}
#endif // __MSP430F5529__
#endif
