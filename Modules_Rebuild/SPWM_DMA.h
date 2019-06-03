#ifndef SPWM_DMA
#define SPWM_DMA
#include <msp430.h>
#include "DDS.h"
#include "../Modules/DMA.h"
extern uint16_t* DMA_SPI_Buffer;
void Setup_SPWM_DMA(){
    SetupSPWMTrigger((uintptr_t)DMA_SPI_Buffer,2500);
    ChangeSinProp(75,1);
}
#endif
