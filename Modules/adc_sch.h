#ifndef __ADC_SCH_H__
#define __ADC_SCH_H__
#include <msp430.h>
#include "stdint.h"
#include "../TIDriver/ucs.h"
#include <stdlib.h>
#include "profile.h"

#if ENABLE_VPP_DETECT
uint16_t V_max = 0;
uint16_t V_min = 0xffff;
#endif
bool ADC_SCH_Flag = 0;
void ADC12_init()
{
    P6DIR |= ADC12_SCH_PxBIT+BIT0+BIT2;
    P6OUT |= ADC12_SCH_PxBIT+BIT0+BIT2;
    P6SEL |= ADC12_SCH_PxBIT+BIT0+BIT2;
    UCS_setExternalClockSource(32768, 4000000L);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    ADC12CTL0 = ADC12REF2_5V + ADC12REFON + ADC12ON + ADC12SHT0_8 + ADC12MSC;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_3;
    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_3;
    ADC12MCTL1 = ADC12INCH_1;
    ADC12MCTL2 = ADC12INCH_2 + ADC12EOS ;
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;
}
#endif // !__ADC_SCH_H__
