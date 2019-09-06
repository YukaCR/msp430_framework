#ifndef __VS_CODE__
#define __VS_CODE_H__ // FORCE Disable DCOCLK, main do any thing startup
#endif                // reverse some linting abilities

#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
#include "TIDriver/ucs.h"
#include "Final/servo.h"

// ACLK 32768 Hz
// SMCLK 32768 Hz
// MCLK 1MHz -> 32768 Hz

inline void ServoRightInc()
{
    TA1CCR1 += 1;
    if (TA1CCR1 > 25)
    {
        TA1CCR1 = 25;
    }
}
inline void ServoLeftInc()
{
    TA1CCR1 -= 1;
    if (TA1CCR1 < 5)
    {
        TA1CCR1 = 5;
    }
}

uint16_t value =    20;
int main()
{
    // 1MHz init.
    WDTCTL = WDTPW | WDTHOLD;
    // Subsystem clock init
    UCS_initClockSignal(UCS_ACLK, UCS_VLOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    // Servo init
    P2SEL |= BIT0;
    P2DIR |= BIT0;
    P1REN |= BIT5;
    P1OUT |= BIT5;
    TA1CTL = TASSEL__ACLK | ID__1 | MC__STOP;
    TA1CCR0 = 100;
    TA1CCR1 = 22; // Center value
    TA1CCTL0 = CM_1 | SCS | SCCI;
    TA1CCTL1 = CM_0 | SCS | OUTMOD_6;

    P2SEL |= BIT5;
    P2DIR |= BIT5;
    P2REN |= BIT5 | BIT3;
    P2OUT |= BIT5 | BIT3;
    TA2CTL = TASSEL__SMCLK | ID__1 | MC__STOP;
    TA2CCR0 = 100;
    TA2CCR2 = 60; // Center value
    TA2CCTL0 = CM_1 | SCS | SCCI;
    TA2CCTL2 = CM_0 | SCS | OUTMOD_6;

#if 0
        // servo max:  82, min: 17, center: 49.
        // servo limit: 30-150, 
    TA1CCTL0 = CM_1 | SCS | SCCI;
    TA1CCTL1 = CM_0 | SCS | OUTMOD_2;

    // Setup sampling using TimerA2
    TA2CTL = TASSEL__SMCLK | ID__1 | MC__STOP;
    TA2CCR0 = 65;
    TA2CCR2 = 55; 
    TA2CCTL0 = CM_1 | SCS | SCCI;
    TA2CCTL2 = CM_0 | SCS | OUTMOD_6;

    // TA2 DMA
    DMACTL0 |= DMA0TSEL__TA2CCR0;
    DMA0CTL = DMADT_4 | DMADSTINCR_3 | DMASRCBYTE | DMADSTBYTE;
    __data20_write_long(((uintptr_t)&DMA2SA, (uintptr_t)&P1IN);
    __data20_write_long((uintptr_t)&DMA2DA, (uintptr_t)p1data);
    DMA0SZ = 10;
#endif
    TA1CTL |= MC__UP;
    TA2CTL |= MC__UP;
    P2IE &= ~BIT3;
    P1IE &= ~BIT5;

    // nice data
    while (1)
    {
        if (TA1CTL & TAIFG)
        {
            TA1CTL &= ~TAIFG;
            if (!(P2IN & BIT3))
            {
                TA1CCR1 = value;
            }
            else if (!(P1IN & BIT5))
            {
                TA1CCR1 = value - 3;
            }
            else{

            }
        }
    }
    __bis_SR_register(CPUOFF);
}
#if 0
uint16_t i = 5000;
#pragma vector = TIMER2_A1_VECTOR
interrupt void TA2ISR(){
    if(!i--){
        uint8_t j = 700;
        TA2CCR2 = TA2CCR0; 
        while(j--){
            TA2CTL &=~ TAIFG;
            while (!TA2CTL&TAIFG);
        }
        i = 10000;
        TA2CCR2 = 200; 
    }
    TA2CCR0 &=~ TAIFG;
}

#endif
