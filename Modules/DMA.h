#ifndef DMA_H
#define DMA_H
#include <msp430.h>
#include "../vscode.h"
#include <stdint.h>
#include "MPY32.h"
#include "../TIDriver/dma.h"
/*
*   3 DMA Channels with 5529
*   DMA0 used with MPY32
*   DMA2 used with OLED
*   DMA0,DMA1 used with DAC_DDS
*/
bool DMA0_IN_USE;
#define CS_BIT BIT5
#define LoadDACs_BIT BIT6
#define STE_BIT BIT2
#define GEN_SPI_PIN BIT3 + BIT4
uint8_t P3PortData[2] = {GEN_SPI_PIN  + CS_BIT + LoadDACs_BIT, GEN_SPI_PIN + STE_BIT + LoadDACs_BIT};
void SetupMPYTriger(){
  DMACTL0 |= DMA0TSEL_29;
  DMA0_IN_USE= 1;
  DMACTL4 = DMARMWDIS;                      // Read-modify-write disable
  DMA0CTL &= ~DMAIFG;
  DMA0CTL = DMADT_4+DMAEN+DMADSTINCR_3; // Rpt single tranfer, inc dst, Int
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) &RESLO);
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &MPY32_RESULT);
  DMA0SZ = 1;                               // DMA0 size = 1
  __enable_interrupt();
}
#define DisableMPYTriger DMA0CTL &=~ DMAEN;
#define EnableMPYTriger DMA0CTL |= DMAEN;
void SetupDDSTrigger(uintptr_t DDSMemory,uintptr_t PortData){
    DMACTL0 = 0;
    DMACTL0 |= DMA0TSEL__USCIA0TX;
    DMACTL4 = DMARMWDIS;                      // Read-modify-write disable
    DMA0CTL = DMADT_5 + DMASRCINCR_3 + DMADSTBYTE + DMASRCBYTE + DMAEN + DMAIE;
    __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) PortData);
    __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &P3OUT);
    DMA0SZ = 2;
    DMACTL0 |= DMA1TSEL__DMA0IFG;
    DMA1CTL = DMADT_4 + DMASRCINCR_3 + DMADSTBYTE + DMASRCBYTE + DMAEN;
    __data20_write_long((uintptr_t) &DMA1SA,(uintptr_t) DDSMemory );
    __data20_write_long((uintptr_t) &DMA1DA,(uintptr_t) &UCA0TXBUF);
    DMA1SZ = 5000;
    __enable_interrupt();
}
void SetupOLEDTrigger( uintptr_t GraphicsMemory,uint16_t GraphicsMemorySize){
  DMACTL1 |= DMA2TSEL__USCIB0TX;
  DMA2CTL = DMADT_4 + DMASRCINCR_3 + DMADSTBYTE + DMASRCBYTE + DMAEN;
  __data20_write_long((uintptr_t) &DMA2SA,(uintptr_t) GraphicsMemory);
  __data20_write_long((uintptr_t) &DMA2DA,(uintptr_t) &UCB0TXBUF);
  DMA2SZ = GraphicsMemorySize;
}
#pragma vector = DMA_VECTOR
interrupt void DMA_ISR(){
    DMA0CTL &=~ DMAIFG;
    *(P3PortData) ^=  LoadDACs_BIT;
}
#endif
