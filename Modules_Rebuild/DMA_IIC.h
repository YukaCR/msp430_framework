/*
*  Author: YukaCR 
*  DMA IIC For screens.
*  mainly use for ssd1306.
*  example: Source/Task6.cpp 
*/
#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "../TIDriver/dma.h"
#include "../TIDriver/usci_b_i2c.h"
#include "../TIDriver/gpio.h"
#include "../Modules/DMA.h"
#include "IIC.h"
#define DMA_CHANNEL DMA_CHANNEL_2
#define DMA_UCB1TXIFG DMA_TRIGGERSOURCE_23
#define DMA_UCB0TXIFG DMA_TRIGGERSOURCE_19
#define DMA_TRIGGERSOURCE  DMA_UCB0TXIFG
#define UCBxBASE USCI_B0_BASE
void setupDMAIIC(uintptr_t _GraphicsMemory,uint16_t _MemorySize){
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL1 |= UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL1 &=~ UCSWRST;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x00;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x22;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0xFF;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x21;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0;
    while(!UCB0IFG&UCTXIFG);
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL1 |= UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL1 &=~ UCSWRST;
    UCB0CTL1 |= UCTXSTT;
          while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x00;
    UCB0IFG&=~UCTXIFG;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 127;
    UCB0IFG&=~UCTXIFG;
    while(!UCB0IFG&UCTXIFG);
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL1 |= UCSWRST;
    UCB0CTL1 &=~ UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    UCB0IFG &=~ UCTXIFG;
    SetupOLEDTrigger(_GraphicsMemory,_MemorySize);
    UCB0TXBUF = 0x40;
    __delay_cycles(1000);//1ms

    if(USCI_B_I2C_getInterruptStatus(UCBxBASE,USCI_B_I2C_NAK_INTERRUPT)){
        __no_operation();
    }
}
