#ifndef DMA_H
#define DMA_H
#include <msp430.h>
#include "../vscode.h"
#include <stdint.h>
#include "MPY32.h"
/*
*   3 DMA Channels with 5529
*   DMA0 used with MPY32.H
*/
void SetupMPYTriger(){
  DMACTL0 |= DMA0TSEL_29;
  DMACTL4 = DMARMWDIS;                      // Read-modify-write disable
  DMA0CTL &= ~DMAIFG;
  DMA0CTL = DMADT_4+DMAEN+DMADSTINCR_3; // Rpt single tranfer, inc dst, Int
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) &RESLO);
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &MPY32_RESULT);
  DMA0SZ = 4;                               // DMA0 size = 1
  __enable_interrupt();
}

#pragma vector = DMA_VECTOR
interrupt void DMA_ISR(){
    if(DMAIV == DMAIV_DMA0IFG){
        MPY_BUSY = 0;
    }
}
#endif
