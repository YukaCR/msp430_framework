#include "DPM_PWMControl.h"
#include "../Modules/MPY32.h"
#include "../IQMath/IQmathLib.h"

#if defined(DMA0_INUSE)|| defined(TimerA2_INUSE)
#error "[DPM_PWMControl_92MHz_40kHz.h]: Perhenial already in use."
#endif
#define DMA0_INUSE 1

inline void DPM_PWMC_Change_Freqency(DPM_DATA_t value){
    __no_operation();//no need
    return;
}
inline void DPM_PWMC_Change_DutyCycle(DPM_DATA_t percent){
    TA2CCR2 = TA2CCR1 = _Q12mpy(2048, percent);
    return;
}
inline void DPM_PWMC_Setup_PWMControl(){
    // setup DMA controll
    DMACTL0 |= DMA0TSEL__TA2CCR0;
    DMACTL4 = DMARMWDIS;
    DMA0CTL = DMADT_5 | DMASRCINCR_3 | DMADSTINCR_3 | DMAEN;
    __data20_write_long((uintptr_t)&DMA0DA, (uintptr_t)&TA2CCR1);
    __data20_write_long((uintptr_t)&DMA0SA, (uintptr_t)&DPM_Current_DutyCycle);
    DMA0SZ = 2;
    DPM_Current_DutyCycle.TA2CCR2 = 0;
    DPM_Current_DutyCycle.TA2CCR1 = 0;

    // assert ucs at max speed.
    P2SEL |= BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT5 | BIT4;
    TA2CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA2CCR0 = 2048;
    TA2CCTL0 = CM_1 | SCS | SCCI;
    TA2CCTL1 = CM_0 | SCS | DPM_PWMControl_PWM1_OUTMODE;
    TA2CCTL2 = CM_0 | SCS | DPM_PWMControl_PWM2_OUTMODE;
    TA2CTL |= MC__UP | TACLR;
}
inline void DPM_PWMC_Channel1_DutyCycle_Increase(DPM_DATA_t value = 1){
    if(DPM_Current_DutyCycle.TA2CCR1 > 1700){return;};
    DPM_Current_DutyCycle.TA2CCR1 += value;
}
inline void DPM_PWMC_Channel1_DutyCycle_Decrease(DPM_DATA_t value = 1){
    if(DPM_Current_DutyCycle.TA2CCR1 < 2){return;};
    DPM_Current_DutyCycle.TA2CCR1 -= value;
}
inline void DPM_PWMC_Channel2_DutyCycle_Increase(DPM_DATA_t value = 1){
    DPM_Current_DutyCycle.TA2CCR2 += value;
}
inline void DPM_PWMC_Channel2_DutyCycle_Decrease(DPM_DATA_t value = 1){
    DPM_Current_DutyCycle.TA2CCR2 -= value;
}
#undef TA2CCR2
#undef TA2CCR1
#define TA2CCR2 DPM_Current_DutyCycle.TA2CCR2
#define TA2CCR1 DPM_Current_DutyCycle.TA2CCR1