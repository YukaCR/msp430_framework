/*#include "TIDriver/ucs.h"
#include "msp430.h"
#include "UCS.h"
#ifdef VS_CODE
#define interrupt
#endif

#if defined(USE_TA0_OVERFLOW_ISR) || defined(USE_TA0_CCR1_4_ISR)
inline void TA0_OV_CCR_ISR();
#pragme vector = TIMER0_A1_VECTOR
interrupt void T0A1_ISR(){
    TA0_OF_CCR_ISR();
    TA0CTL &=~ TAIFG;
    TA0CCR1 &=~ CCIFG;
    TA0CCR2 &=~ CCIFG;
    TA0CCR3 &=~ CCIFG;
}
#endif
#if defined(USE_TA0_CCR0_ISR)
inline void TA0CCR0_ISR();
#pragma vector = TIMER0_A0_VERTOR
interrupt void T0A0_ISR(){
    TA0CCR0_ISR();
    TA0CCTL0 &=~ CCIFG;
}
#endif
#if defined(USE_TA1_OVERFLOW_ISR) || defined(USE_TA1_CCR1_2_ISR)
inline void TA1_OV_CCR_ISR();
#pragma vector = TIMER1_A1_VERTOR
interrupt void T1A1_ISR(){
    TA1_OV_CCR_ISR();
}
#endif
#if defined(USE_TA1_CCR0_ISR)
inline void TA1CCR0_ISR();
#pragma vector = TIMER1_A0_VERTOR
interrupt void T1A0_ISR(){
    TA1CCR0_ISR();
}
#endif
#if defined(USE_TB0_OVERFLOW_ISR) || defined(USE_TB0_CCR1_6_ISR)
inline void TB0_OV_CCR_ISR();
#pragma vector = TIMER0_B1_VERTOR
interrupt void T0B1_ISR(){
    TB0_OV_CCR_ISR();
}
#endif
#if defined(USE_TB1_CCR0_ISR)
inline void TB0CCR0_ISR();
#pragma vector = TIMER0_B0_VECTOR
interrupt void T0B0_ISR(){
    TB0CCR0_ISR();

}
#endif

void InitTimer0A0(bool USE_ACLK = false){
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0,UCS_XCAP_3);
    UCS_setExternalClockSource(32768,4*Mhz);
    Clock_Init(XT2_ENABLE);
    TA0CTL = USE_ACLK?TASSEL_1:TASSEL_2 + ID_0 + MC_0 + TACLR;
#if defined(USE_TA0_OVERFLOW_ISR)
    TA0CTL0 |= TAIE;
    __enable_interrupt();
#endif
}
enum TIMER_MODE{
    STOP,UP,CONTINUOIS,UP_DOWN
};
void Start_Timer0_A0(TIMER_MODE MODE = UP){
    TA0CTL |= MODE << 6 + TACLR;
}*/
