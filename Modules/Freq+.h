/*
*   Author: YukaCR
*   equal precision frequency measurement using TimerAxCLK.
*   assume input sign as clock signal
*   200Hz ~ 2Mhz 1%
*/

#ifndef __Freq_Plus_H__
#define __Freq_Plus_H__
#include <msp430.h>
#include <stdint.h>
#include "../profile.h"
#include "../TIDriver/ucs.h"
#include "../TIDriver/timer_a.h"

double FreqResult = 0x00;
uint16_t CounterOverflow = 0;
int16_t GateOverflow = 100;
int16_t GateStartOV = 0;
uint16_t GateStartR = 0;
#if CalcPWMPercent
uint8_t Freq_stage = 0;
double Freq_Percent = 0;
bool Freq_Percent_Flag = 0;
#else
bool Freq_stage = 0;
#endif
bool Freq_Flag = 0;
inline void FreqInit();
inline void FreqStart();
inline void FreqStop();
void CalculateFreq();
void (*FreqCallback)(double) = nullptr;
#if CalcPWMPercent
void (*FreqPercentCallback)(double) = nullptr;
#endif
void FreqStop()
{
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    P2SEL &= ~BIT2; //TA2CLK
#else
    P1SEL &= ~BIT6; //TA1CLK
    P1IE &= ~BIT6;  //TA1CLK
#endif
    Freq_Counter_TAxCTL = TASSEL__TACLK + ID_0 + MC__STOP;
    Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__STOP;
    Freq_Gate_TAxCCR0 = 40000;
}
void FreqInit()
{
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    P2SEL |= BIT2; //TA2CLK
#else
    P1SEL |= BIT6;  //TA1CLK
#endif
    UCS_setExternalClockSource(32768, 4 * Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_3);
    UCS_setExternalClockSource(32768, 4 * Mhz);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    P2IES &= ~BIT2; //TA2CLK
#else
    P1IES &= ~BIT6; //TA1CLK
#endif

    Freq_Counter_TAxCTL = TASSEL__TACLK + ID_0 + MC__STOP;
    Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__STOP;
    Freq_Gate_TAxCCR0 = 40000;
}
void CalculateFreq()
{
    uint32_t Count = CounterOverflow;
    uint32_t Gate = GateStartOV - GateOverflow + 1;
    Count <<= 16;
    Count += (uint32_t)(Freq_Counter_TAxR) + 2;
    Gate *= 40000;
    Gate += Freq_Gate_TAxR;

    FreqResult = (double)Count * ((double)(4000000L) / (double)Gate);
#if !CalcPWMPercent
    Freq_Flag = 1;
#endif
}
void FreqStart()
{
    CounterOverflow = 0;
    GateOverflow = 100;
    GateStartOV = 0;
    GateStartR = 0;
    Freq_stage = 0;
    Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__CONTINUOUS + TAIE + TACLR; //BIS MC__CONTINUOUS, TAIE
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    P2IE |= BIT2; //TA2CLK
#else
    P1IE |= BIT6;   //TA1CLK
#endif
}
inline void REAL_GATE_ISR()
{
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
    P2IE &= ~BIT2; //TA2CLK
#else
    P1IE &= ~BIT6;  //TA1CLK
#endif
    if (Freq_stage)
    { //Stage 1. Stop Counter
#if CalcPWMPercent
        if (Freq_stage == 1)
        {
#endif
            Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__STOP; //GateStop
#if !CalcPWMPercent
            Freq_stage = 0;
            FreqStop();
#endif
            CalculateFreq();
            GateOverflow = 100;
            GateStartOV = 0;
            GateStartR = 0;
            Freq_stage = 3;
            Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__UP + TAIE + TACLR;
#if CalcPWMPercent
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
            P2IES |= BIT2; //TA2CLK
            P2IE |= BIT2;  //TA2CLK
#else
            P1IES &=~ BIT6;  //TA1CLK
            P1IFG = 0X00;
            P1IE |= BIT6;   //TA2CLK
#endif
        }
        else if (Freq_stage == 3)
        {
            GateStartR = Freq_Gate_TAxR;
            GateStartOV = GateOverflow;
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
            P2IES &= ~BIT2; //TA2CLK
            P2IE |= BIT2;   //TA2CLK
#else
            P1IES |= BIT6; //TA1CLK
            P1IFG = 0X00;
            P1IE |= BIT6;   //TA2CLK
#endif
            Freq_stage = 4;
        }
        else if(Freq_stage = 4)
        {
            Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + MC__STOP;
            uint32_t HighTime = 100 - GateStartOV;
            HighTime = (HighTime << 16) + GateStartR;
            uint32_t totalTime = 100 - GateOverflow;
            totalTime = (totalTime << 16) + Freq_Gate_TAxR;
            Freq_Percent = (double)HighTime / (double)totalTime;
            Freq_Percent *= 100;
            FreqStop();
            Freq_Flag = 1;
            Freq_Percent_Flag = 1;
        }
#endif
    }
    else
    {
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
        P2SEL |= BIT2;
#else
        P1SEL |= BIT6;
#endif                                                                              //Stage 0. Start Counter
        Freq_Counter_TAxCTL = TASSEL__TACLK + ID_0 + TACLR + TAIE + MC__CONTINUOUS; //CounterStart.
        GateStartOV = GateOverflow;
        GateStartR = Freq_Gate_TAxR;
        Freq_stage = 1;
    }
}
#pragma vector = Freq_Gate_TIMERx_A1_VECTOR
interrupt void GateISR()
{
    if (!GateOverflow--) //4MHz, 1s
    {
        Freq_Counter_TAxCTL = TASSEL__TACLK + ID_0 + MC__STOP; //BIC TAIE, TAIFG, MC__Stop
#if FREQ_PLUS_COUNTER_TIMER == TIMER_A2_BASE
        P2SEL &= ~BIT2; //Move Operation to P2 ISR
        P2IE |= BIT2;   //BIS P2.2 IE
#else
        P1SEL &= ~BIT6;
        P1IE |= BIT6; //BIS P1.6 IE
#endif
    };
    Freq_Gate_TAxCTL = TASSEL__SMCLK + ID_0 + TAIE + MC__UP + TACLR; //Clear TAIFG
}

#pragma vector = Freq_Counter_TIMERx_A1_VECTOR
interrupt void CounterISR()
{
    CounterOverflow++;
    Freq_Counter_TAxCTL = TASSEL__TACLK + ID_0 + MC__CONTINUOUS + TAIE;
}
#endif
