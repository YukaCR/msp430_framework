
#include "DPM_Meausure.h"

#define DPM_Measure_Factor _Q12(2.5)

inline void DPM_Current_Detect(){
    DPM_Current = _Q12mpy(ADC12MEM0,_Q12(DPM_ADC_Reference_Voltage/DPM_Current_Sense_Coff));
}
inline void DPM_Voltage_Detect(){
    DPM_Voltage = _Q12mpy(ADC12MEM1,_Q12(DPM_ADC_Reference_Voltage/DPM_Voltage_Sense_Coff));
}
inline void DPM_Power_Detect(){
#if DPM_Output_Mode == DPM_Output_Mode_Power
    DPM_Current_Detect();
    DPM_Voltage_Detect();
    DPM_Power = _Q12mpy(DPM_Voltage,DPM_Current);
#endif
}
void DPM_Setup_Current_Measure()
{
    P6DIR |= BIT0;
    P6OUT |= BIT0;
    P6SEL |= BIT0;
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 =  ADC12REF2_5V + ADC12REFON + ADC12ON + ADC12SHT0_4 + ADC12MSC;
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1 | ADC12SSEL_1;
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_0;
   // ADC12IE |= ADC12IE0;
    ADC12CTL0 |= ADC12ENC | ADC12SC;
}
void DPM_Setup_Voltage_Measure()
{
    P6DIR |= BIT1;
    P6OUT |= BIT1;
    P6SEL |= BIT1;
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 =  ADC12REF2_5V + ADC12REFON + ADC12ON + ADC12SHT0_4 + ADC12MSC;
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1 | ADC12SSEL_1;
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_1;
   // ADC12IE |= ADC12IE1;
    ADC12CTL0 |= ADC12ENC | ADC12SC;
}
void DPM_Setup_Power_Measure()
{
    P6DIR &=~ BIT1 + BIT0;
    P6OUT &= BIT1 + BIT0;
    P6SEL |= BIT1 + BIT0;
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 =  ADC12REF2_5V + ADC12REFON + ADC12ON + ADC12SHT0_2 + ADC12MSC;
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_3 | ADC12SSEL_1;
    ADC12MCTL0 = ADC12SREF_1 + ADC12INCH_0;
    ADC12MCTL1 = ADC12SREF_1 + ADC12INCH_1 + ADC12EOS;
    //ADC12IE |= ADC12IE1;
    ADC12CTL0 |= ADC12ENC | ADC12SC;
}
