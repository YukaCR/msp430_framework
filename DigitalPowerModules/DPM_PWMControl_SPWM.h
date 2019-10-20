/*
*   Author: YukaCR
*   sukoi no dpm pwmc spwm module
*   maybe the best spwm generation module for msp430.
*/
#include "DPM_PWMControl.h"
#include "../Modules/MPY32.h"
#include "../IQMath/IQmathLib.h"
#include "../IQMath/QmathLib.h"
#if defined(DMA0_INUSE) || defined(DMA1_INUSE) || defined(TimerA2_INUSE)
#error "[DPM_PWMControl_SPWM.h]: Perhenial already in use."
#endif 
#define DMA0_INUSE    1
#define DMA1_INUSE    1
#define TimerA2_INUSE 1
 
#define P2_4_INUSE
#define P2_5_INUSE


const uint16_t data_pwm_test[402] = {257,261,265,269,272,276,280,283,287,291,295,298,301,305,309,312,316,320,324,327,331,334,337,341,345,348,351,355,358,362,365,368,372,375,378,381,385,388,391,394,397,400,403,406,409,412,415,418,420,423,426,428,431,433,436,438,441,443,445,447,450,452,454,456,459,460,462,464,466,468,470,471,472,474,475,477,479,480,481,483,484,485,486,486,487,488,489,490,491,492,492,493,493,494,494,495,495,495,495,495,496,495,495,495,495,495,494,494,493,493,492,492,491,490,489,488,487,486,486,485,484,483,481,480,479,477,475,474,472,471,470,468,466,464,462,460,459,456,454,452,450,447,445,443,441,438,436,433,431,428,426,423,420,418,415,412,409,406,403,400,397,394,391,388,385,381,378,375,372,368,365,362,358,355,351,348,345,341,337,334,331,327,324,320,316,312,309,305,301,298,295,291,287,283,280,276,272,269,265,261,256,253,249,245,242,238,234,230,227,223,219,216,213,209,205,202,198,194,190,187,183,180,176,173,169,166,162,159,156,152,149,146,142,139,135,133,129,126,123,120,117,114,111,108,105,102,99,96,94,91,88,86,83,81,78,76,73,71,68,67,64,62,60,58,55,54,52,50,48,46,44,43,41,40,39,37,35,34,33,31,30,29,28,27,27,26,25,24,23,22,22,21,21,20,20,19,19,19,19,19,19,19,19,19,19,19,20,20,21,21,22,22,23,24,25,26,27,27,28,29,30,31,33,34,35,37,39,40,41,43,44,46,48,50,52,54,55,58,60,62,64,67,68,71,73,76,78,81,83,86,88,91,94,96,99,102,105,108,111,114,117,120,123,126,129,133,135,139,142,146,149,152,156,159,162,166,169,173,176,180,183,187,190,194,198,202,205,209,213,216,219,223,227,230,234,238,242,245,249,253,257,261};
uint32_t DPM_FLL_Base = _IQ30( 1.0 / 250000.0); // 250k

inline void DPM_PWMC_Change_Freqency(DPM_DATA_t freq){
    DPM_Current_Freqency = freq;
    volatile uint32_t targetFreq;
    register uint16_t targetFreq_;
    HW_MPY_16_16_STA_U(401, freq, targetFreq);
    HW_MPY_16_16_STA_U(TA2CCR0, targetFreq , targetFreq);
    targetFreq_ = targetFreq>>16;
    if(targetFreq_ > 46300000 >> 16){//46.3  MHz to 96MHz
        UCSCTL2 = _IQ30mpy(targetFreq , DPM_FLL_Base);
        UCSCTL5 = DIVA__4 | DIVS__1 | DIVM__4;
        TA2CTL &=~ ID_3; 
        TA2CTL |= ID__1;
    }
    else if(targetFreq_ > 23150000 >> 16){// 33MHz to 66MHz
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 1;
        UCSCTL5 = DIVA__4 | DIVS__1 | DIVM__4;
        TA2CTL &=~ ID_3; 
        TA2CTL |= ID__2;
    }
    else if(targetFreq_ > 11575000 >> 16){// 33MHz to 16.5MHz
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq , DPM_FLL_Base)) << 2;
        UCSCTL5 = DIVA__4 | DIVS__1 | DIVM__4;
        TA2CTL &=~ ID_3; 
        TA2CTL |= ID__4;
    }
    else if(targetFreq_ >  5787500 >> 16){// 16.5MHz to 8.25MHz 
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq , DPM_FLL_Base))<< 3;
        UCSCTL5 = DIVA__4 | DIVS__1 | DIVM__4;
        TA2CTL &=~ ID_3; 
        TA2CTL |= ID__8;
    }
    else if(targetFreq_ >  2893750 >> 16){// 8.25MHz to 4.125MHz 
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 4;   // WAIT FLL Sattle?
        UCSCTL5 = DIVA__4 | DIVS__2 | DIVM__4;
        TA2CTL &=~ ID_3; 
        TA2CTL |= ID__8;
    }
    else if(targetFreq_ >  1446875 >> 16){
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 5;
        UCSCTL5 = DIVA__4 | DIVS__4 | DIVM__4;
        TA2CTL &=~ ID_3;
        TA2CTL |= ID__8;
    }
    else if(targetFreq_ >   723437 >> 16){
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 6;
        UCSCTL5 = DIVA__4 | DIVS__8 | DIVM__4;
        TA2CTL &=~ ID_3;
        TA2CTL |= ID__8;
    }
    else if(targetFreq_ >   361718 >> 16){
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 7;
        UCSCTL5 = DIVA__4 | DIVS__16 | DIVM__4;
        TA2CTL &=~ ID_3;
        TA2CTL |= ID__8;
    }
    else {
        UCSCTL2 = (uint16_t)(_IQ30mpy(targetFreq, DPM_FLL_Base)) << 8;
        UCSCTL5 = DIVA__4 | DIVS__32 | DIVM__4;
        TA2CTL &=~ ID_3;
        TA2CTL |= ID__8;
    }
}   
inline void DPM_PWMC_Change_DutyCycle(DPM_DATA_t dutyCyclePercent){
    TA2CCR0 = _Q12div(512, dutyCyclePercent);
    DPM_PWMC_Change_Freqency(DPM_Current_Freqency);
}
inline void DPM_PWMC_Channel1_DutyCycle_Increase(DPM_DATA_t value = 1){
    TA2CCR0 += value;
    DPM_PWMC_Change_Freqency(DPM_Current_Freqency);
}
inline void DPM_PWMC_Channel1_DutyCycle_Decrease(DPM_DATA_t value = 1){
    TA2CCR0 -= value;
    DPM_PWMC_Change_Freqency(DPM_Current_Freqency);
}
inline void DPM_PWMC_Channel2_DutyCycle_Increase(){
    return;
}
inline void DPM_PWMC_Channel2_DutyCycle_Decrease(){
    return;
}
void DPM_PWMC_Setup_PWMControl(){
    P2SEL |= BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT5 | BIT4;
    TA2CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA2CCR0 = 512;
    TA2CCTL0 = CM_1 | SCS | SCCI;
    TA2CCTL1 = CM_0 | SCS | DPM_PWMControl_PWM1_OUTMODE;
    TA2CCTL2 = CM_0 | SCS | DPM_PWMControl_PWM2_OUTMODE;

    DMACTL0 |= DMA0TSEL__TA2CCR0;
    DMACTL4 = DMARMWDIS; // Read-modify-write disable
    DMA0CTL &= ~DMAIFG;
    __data20_write_long((uintptr_t)&DMA0SA, (uintptr_t)data_pwm_test);
    __data20_write_long((uintptr_t)&DMA0DA, (uintptr_t)&TA2CCR2);
    DMA0CTL = DMADT_4 + DMAEN + DMASRCINCR_3 ; // Rpt single tranfer, inc dst, Int
    DMA0SZ = (sizeof(data_pwm_test)/sizeof(uint16_t)) - 2; // DMA0 size = 1
    uint8_t i = 200;
    while(i--){
        TA2CCTL0 |= CCIFG;
        TA2CCTL0 &=~ CCIFG;
    } 

    DMACTL0 |= DMA1TSEL__TA2CCR0;
    DMA1CTL &= ~DMAIFG;
    __data20_write_long((uintptr_t)&DMA1SA, (uintptr_t)(data_pwm_test + 2));
    __data20_write_long((uintptr_t)&DMA1DA, (uintptr_t)&TA2CCR1);
    DMA1CTL = DMADT_4 + DMAEN + DMASRCINCR_3 ; // Rpt single tranfer, inc dst, Int
    DMA1SZ = (sizeof(data_pwm_test)/sizeof(uint16_t)) - 2; // DMA0 size = 1

    DPM_PWMC_Change_Freqency(50);

    TA2CTL |= TACLR | MC__UP;
}
