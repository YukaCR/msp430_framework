#include "DPM_PWMControl.h"

#if defined(DMA0_INUSE) || defined(DMA1_INUSE) || defined(TimerA2_INUSE)
#error "[DPM_PWMControl_SPWM.h]: Perhenial already in use."
#endif 

#define DMA0_INUSE    1
#define DMA1_INUSE    1
#define TimerA2_INUSE 1

#define P2_


void DPM_PWMC_Setup_PWMControl(){

}