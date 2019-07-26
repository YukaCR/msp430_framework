#ifndef DPM_PWMControll
#define DPM_PWMControll
#include "DPM.h"
void DPM_PWMC_Change_Freqency(DPM_DATA_t);
void DPM_PWMC_Change_DutyCycle(DPM_DATA_t);
void DPM_PWMC_Setup_PWMControl();
DPM_DATA_t DPM_Current_Freqency;
#endif