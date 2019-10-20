/*
*  Author: YukaCR
*  Measure input voltage RMS value. 
*/
#ifndef ADC_EV_DMA
#define ADC_EV_DMA
#include "../Modules/adc_sch.h"
void ADC12_EV_init()
{ 
    P6DIR |= BIT3+BIT2+BIT4+BIT0+BIT1;
    P6OUT |= BIT3+BIT2+BIT4+BIT0+BIT1;
    P6SEL |= BIT3+BIT2+BIT4+BIT0+BIT1;
    UCS_setExternalClockSource(32768, 4000000L);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    ADC12CTL0 = ADC12REF2_5V + ADC12REFON + ADC12ON + ADC12SHT0_8 + ADC12MSC;
    ADC12IE |= BIT0;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2;
    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_3;
    HW_MPY_16_16_S_SETUP;
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;
}
#endif
