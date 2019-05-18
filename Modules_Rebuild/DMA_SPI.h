#ifndef DMA_SPI_H
#define DMA_SPI_H
#define ENABLE_DDS
#include "../TIDriver/ucs.h"
#include "../TIDriver/usci_a_spi.h"
#include "../Modules/DMA.h"
extern volatile uint8_t DMA_SPI_Buffer[5001];//7kb?????
//max at 4M Speed, ksps, 10kHz, 25dotRate
//min at 4M Speed, 250ksps, 100Hz, 2500 dotRate
void setup_DMA_SPI(){
    USCI_A_SPI_initMasterParam spi_param;
    P3SEL |= BIT4+BIT3+BIT2;
    P3DIR |= BIT4+BIT3+BIT2+CS_BIT+LoadDACs_BIT;
    P3OUT |= BIT4+BIT3+BIT2+CS_BIT+LoadDACs_BIT;
    P2DIR |= BIT7;    P2SEL |= BIT7;
    UCA0CTL1 |= UCSWRST;
    spi_param.clockPhase =  USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    spi_param.clockSourceFrequency = 4000000;
    spi_param.desiredSpiClock = 4000000;
    spi_param.msbFirst = USCI_A_SPI_MSB_FIRST;
    spi_param.selectClockSource= USCI_A_SPI_CLOCKSOURCE_SMCLK;
    USCI_A_SPI_initMaster(USCI_A0_BASE,&spi_param);
    UCA0CTL0&=~ UCMODE_3;
    UCA0CTL0|= UCMODE_2 + UCSYNC;
    USCI_A_SPI_enable(USCI_A0_BASE);
    UCA0CTL1 &=~ UCSWRST;
    SetupDDSTrigger((uintptr_t)DMA_SPI_Buffer,(uintptr_t)P3PortData);
    UCA0IFG &=~ UCTXIFG;
    UCA0IFG |= UCTXIFG; 
}
#endif
