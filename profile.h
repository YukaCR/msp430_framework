
#define ENSURE_USE_USCI_A_SPI_INTERRUPT_MODE 1
#include <msp430.h>
#define FREQ_PLUS_COUNTER_TIMER TIMER_A1_BASE
#define ENABLE_PWN 1
#define Mhz 1000000L
#define EnableButtonMartix 0
//ADC_SCH.h
#if defined(__ADC_SCH_H__)
#define ENABLE_VDC_DETECT 1
#define ENABLE_VPP_DETECT 0
#define ENABLE_VRMS_DETECT 0
#define ADC12_SCH_MCTL ADC12MCTL0
#define ADC12_SCH_MEM ADC12MEM0
#define ADC12_SCH_INCHx ADC12INCH_6
#define ADC12_SCH_PxBIT (BIT0 << (ADC12_SCH_INCHx - 1))
#endif // DEBUG
//PWM.h
#if defined(PWM_H)
#define ENABLE_BUCK 1
#define BUCK_INV 1522
#define BUCK_INVC 10
#ifndef _PROFILE_PWN_H_
#define _PROFILE_PWN_H_
#define PWM_PercentOFF 0 //40k
#define EnablePercentOFF ENABLE_BUCK
#define PWN_TAxBASE TIMER_A2_BASE
#define PWM_TAxCTL TA2CTL     /* Timer2_A3 Control */
#define PWM_TAxCCTL0 TA2CCTL0 /* Timer2_A3 Capture/Compare Control 0 */
#define PWM_TAxCCTL1 TA2CCTL1 /* Timer2_A3 Capture/Compare Control 1 */
#define PWM_TAxCCTL2 TA2CCTL2 /* Timer2_A3 Capture/Compare Control 2 */
#define PWM_TAxR TA2R         /* Timer2_A3 */
#define PWM_TAxCCR0 TA2CCR0   /* Timer2_A3 Capture/Compare 0 */
#define PWM_TAxCCR1 TA2CCR1   /* Timer2_A3 Capture/Compare 1 */
#define PWM_TAxCCR2 TA2CCR2   /* Timer2_A3 Capture/Compare 2 */
#define PWM_TAxIV TA2IV       /* Timer2_A3 Interrupt Vector Word */
#define PWM_TAxEX0 TA2EX0     /* Timer2_A3 Expansion Register 0 */
#define PWM_TIMERx_A0_VECTOR TIMER2_A0_VECTOR
#define PWM_TIMERx_A1_VECTOR TIMER2_A1_VECTOR
#define Use4MHzCrystal 1
#define PWM1MODE OUTMOD_2 //OUTMOD_6 || OUTMOD_2(REVERSE)
#define EnablePWM2 1
#define PWM2MODE OUTMOD_2 //OUTMOD_6 || OUTMOD_2(REVERSE)
#if ENABLE_BUCK
#define PWMFreq 40000L
#else
#define PWMFreq 1000L
#endif
#define MHz 1000000L
#define EnableSPWM 0
#endif
#endif
//Freq+.h
#if defined(__Freq_Plus_H__)
#ifndef _PROFILE_Freq_H_
#define _PROFILE_Freq_H_
#define CalcPWMPercent 1
#define FREQ_PLUS_COUNTER_TIMER TIMER_A1_BASE
#define FREQ_PLUS_GATE_TIMER TIMER_A0_BASE
#define Freq_Counter_TAxCTL TA1CTL     /* Timer2_A3 Control */
#define Freq_Counter_TAxCCTL0 TA1CCTL0 /* Timer2_A3 Capture/Compare Control 0 */
#define Freq_Counter_TAxCCTL1 TA1CCTL1 /* Timer2_A3 Capture/Compare Control 1 */
#define Freq_Counter_TAxCCTL2 TA1CCTL2 /* Timer2_A3 Capture/Compare Control 2 */
#define Freq_Counter_TAxR TA1R         /* Timer2_A3 */
#define Freq_Counter_TAxCCR0 TA1CCR0   /* Timer2_A3 Capture/Compare 0 */
#define Freq_Counter_TAxCCR1 TA1CCR1   /* Timer2_A3 Capture/Compare 1 */
#define Freq_Counter_TAxCCR2 TA1CCR2   /* Timer2_A3 Capture/Compare 2 */
#define Freq_Counter_TAxIV TA1IV       /* Timer2_A3 Interrupt Vector Word */
#define Freq_Counter_TAxEX0 TA1EX0     /* Timer2_A3 Expansion Register 0 */
#define Freq_Counter_TIMERx_A0_VECTOR TIMER1_A0_VECTOR
#define Freq_Counter_TIMERx_A1_VECTOR TIMER1_A1_VECTOR
#define Freq_Gate_TAxCTL TA0CTL     /* Timer2_A3 Control */
#define Freq_Gate_TAxCCTL0 TA0CCTL0 /* Timer2_A3 Capture/Compare Control 0 */
#define Freq_Gate_TAxCCTL1 TA0CCTL1 /* Timer2_A3 Capture/Compare Control 1 */
#define Freq_Gate_TAxCCTL2 TA0CCTL2 /* Timer2_A3 Capture/Compare Control 2 */
#define Freq_Gate_TAxR TA0R         /* Timer2_A3 */
#define Freq_Gate_TAxCCR0 TA0CCR0   /* Timer2_A3 Capture/Compare 0 */
#define Freq_Gate_TAxCCR1 TA0CCR1   /* Timer2_A3 Capture/Compare 1 */
#define Freq_Gate_TAxCCR2 TA0CCR2   /* Timer2_A3 Capture/Compare 2 */
#define Freq_Gate_TAxIV TA0IV       /* Timer2_A3 Interrupt Vector Word */
#define Freq_Gate_TAxEX0 TA0EX0     /* Timer2_A3 Expansion Register 0 */
#define Freq_Gate_TIMERx_A0_VECTOR TIMER0_A0_VECTOR
#define Freq_Gate_TIMERx_A1_VECTOR TIMER0_A1_VECTOR
#endif
#endif
//Port Interrupt
