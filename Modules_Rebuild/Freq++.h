/*
*   Author: YukaCR
*   sukoi no equal precision frequency measurement.
*   example: main.cpp
*   also show the ways to extern timer bits from 16 to 16*N(N>1, N<=3)
*   test with msp430f5529. from 10mHz to 120MHz(or more.) 0.01%. 5% at 1mHz
*   but why 76MHz measures 0?
*/
#pragma once
#include <msp430.h>
#include <stdint.h>
#include "../TIDriver/ucs.h"
#include "../IQMath/IQmathLib.h"
#include "../vscode.h"
// predict area.
uint16_t CounterOV;
//uint16_t InputOV;

double Setup_FreqPP()
{ 
    // use 4MHz clock as CTOV;
    // init FreqCounter.
    // 32bit input timer can accept up to 4GHz
    // use TimerA2, DMA1 as a 32bit input tiemr.
    // use TimerA0, DMA0 as a 32bit counter timer.
    // use TimerA1 as a 1s timer.
    // Diagram: TA2OV -> TA1UP; GateOV -> ExtInterrupt;
    // 27byte
    volatile uint8_t temp0 = 0, temp1 = 0;
    volatile uint8_t temp2 = 0, temp3 = 0;
    volatile const uint8_t P2SEL_D = BIT1;
    volatile double result;
    volatile uint32_t ctx, gtx;
    burnDCO();     // fuck dco to 33MHz
    P2SEL = 0X00; // TA2CLK
    P2IE = 0X00;
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);

    TA2CTL = TASSEL__TACLK | ID_0 | MC__STOP | TACLR;
    TA1CTL = TASSEL__ACLK  | ID_0 | MC__STOP | TACLR;
    TA0CTL = TASSEL__SMCLK | ID_0 | MC__STOP | TACLR; // Timer Ready State
    TA0CCTL0 =  CM_1 + SCS + SCCI ;
    TA1CCTL0 =  CM_1 + SCS + SCCI ;
    TA2CCTL0 =  CM_1 + SCS + SCCI ;
    TA1CCR0 = 32768;
    TA2CCR0 = 0XFFFF;
    TA0CCR0 = 0XFFFF;

    DMACTL0 = DMA0TSEL__TA2CCR0 | DMA1TSEL__TA0CCR0; // 32bit timer
    DMACTL4 = DMARMWDIS;
    __data20_write_long((uintptr_t)&DMA0SA, (uintptr_t)&temp0);
    __data20_write_long((uintptr_t)&DMA0DA, (uintptr_t)&temp1);
    __data20_write_long((uintptr_t)&DMA1SA, (uintptr_t)&temp2);
    __data20_write_long((uintptr_t)&DMA1DA, (uintptr_t)&temp3);
    
    DMA0CTL = DMADT_4 + DMADSTBYTE + DMASRCBYTE + DMAEN;
    DMA1CTL = DMADT_4 + DMADSTBYTE + DMASRCBYTE + DMAEN;
    DMA0SZ = 0XFFFF;
    DMA1SZ = 0XFFFF;

    DMACTL1 = DMA2TSEL__TA1CCR0;                     // Gate Interrupt control
    __data20_write_long((uintptr_t)&DMA2SA, (uintptr_t)&P2SEL_D);
    __data20_write_long((uintptr_t)&DMA2DA, (uintptr_t)&P2SEL);
    DMA2CTL = DMADT_5 + DMADSTBYTE + DMASRCBYTE + DMAEN + DMAIE;
    DMA2SZ = 1;
    P2IFG = 0X00;
    while (!(P2IFG & BIT2));
    P2SEL = BIT2; // TA2CLK
    TA0CTL = TASSEL__SMCLK | ID_0 | MC__UP | TACLR;
    TA1CTL = TASSEL__ACLK | ID_0 | MC__UP | TACLR;
    TA2CTL = TASSEL__TACLK | ID_0 | MC__UP | TACLR;
    LPM0; // nothing to do, halt CPU. RTOS: wait for signal.
    while (!(P2IFG & BIT2));
    TA2CTL = TASSEL__TACLK | ID_0 | MC__STOP;
    TA1CTL = TASSEL__ACLK | ID_0 | MC__STOP | TACLR;
    TA0CTL = TASSEL__SMCLK | ID_0 | MC__STOP; // Timer Ready State
    ctx = ~DMA0SZ;
    gtx = ~DMA1SZ;
    ctx <<= 16;
    gtx <<= 16;
    ctx |= TA2R;
    gtx |= TA0R;
    ctx += 1;
    result = (double)ctx * 4000000.0f / (double)gtx;
    return result;
}

#pragma vector = DMA_VECTOR
interrupt void DMA_ISR()
{
    P2IFG = 0X00;
    TA1CTL = TASSEL__ACLK | ID_0 | MC__STOP | TACLR;
    DMA2CTL &=~ DMAIFG;
    LPM0_EXIT;
}
