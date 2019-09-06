#ifndef DPM_PWMControll
#define DPM_PWMControll
#include "DPM.h"


void DPM_PWMC_Change_Freqency(DPM_DATA_t);
void DPM_PWMC_Change_DutyCycle(DPM_DATA_t);
void DPM_PWMC_Setup_PWMControl();
void DPM_PWMC_Channel1_DutyCycle_Increase(DPM_DATA_t);
void DPM_PWMC_Channel1_DutyCycle_Decrease(DPM_DATA_t);
void DPM_PWMC_Channel2_DutyCycle_Increase(DPM_DATA_t);
void DPM_PWMC_Channel2_DutyCycle_Decrease(DPM_DATA_t);

DPM_DATA_t DPM_Current_Freqency;
struct DPM_Current_DutyCycle_t{
    DPM_DATA_t TA2CCR1;
    DPM_DATA_t TA2CCR2;
} DPM_Current_DutyCycle = {0,0};
#endif
