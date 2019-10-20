
/*
    Author: YukaCR
    MSB First SPI, Something interesting.
    sukoi no hwspi driver
*/

#ifndef __SPI_H__
#define __SPI_H__
#include "../TIDriver/usci_a_spi.h"
#include "../TIDriver/ucs.h"
#warning "SPI library use DCOCLK as ACLK source, be careful if some modeules use ACLK at 32768Hz"
#define UCAxTXBUF UCA0TXBUF
#define UCAxSTAT  UCA0STAT
#define UCAxRXBUF UCA0RXBUF 
void Setup_SPI(){
    P3OUT |= BIT3 + BIT4;P3SEL |= BIT3+BIT4;P3REN |= BIT3+BIT4;
    P2OUT |= BIT7; P2SEL |= BIT7; P2REN|= BIT7;
    UCS_initClockSignal(UCS_ACLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    USCI_A_SPI_initMasterParam spi_param;
    spi_param.selectClockSource = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    spi_param.msbFirst = USCI_A_SPI_MSB_FIRST;
    //spi_param.clockPhase = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_param.clockPhase = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    spi_param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    spi_param.clockSourceFrequency = 4000000;
    spi_param.desiredSpiClock = 4000000;
    USCI_A_SPI_initMaster(USCI_A0_BASE,&spi_param);
    USCI_A_SPI_enable(USCI_A0_BASE);
};
uint8_t spi_send(uint8_t _data)
{
	UCAxTXBUF = _data; 
	while (UCAxSTAT & UCBUSY);
	return UCAxRXBUF;
}
uint16_t spi_send16(uint16_t _data)
{
    uint16_t spi_result;
	UCAxTXBUF = (_data>>8)&0xff; 
	while (UCAxSTAT & UCBUSY);
    spi_result = (uint16_t)UCAxRXBUF << 8;
	UCAxTXBUF = _data&0xff; 
	while (UCAxSTAT & UCBUSY);
    spi_result |= UCAxRXBUF;
	return spi_result;
}
uint32_t spi_send32(uint32_t _data)
{
    uint32_t spi_result;
	UCAxTXBUF = (_data>>24)&0xff;
	while (UCAxSTAT & UCBUSY);
    spi_result = (uint32_t)UCAxRXBUF << 24;
	UCAxTXBUF = (_data>>16)&0xff;
	while (UCAxSTAT & UCBUSY);
    spi_result |= (uint32_t)UCAxRXBUF << 16;
	UCAxTXBUF = (_data>>8)&0xff;
	while (UCAxSTAT & UCBUSY);
    spi_result |= (uint32_t)UCAxRXBUF << 8;
	UCAxTXBUF = _data&0xff;
	while (UCAxSTAT & UCBUSY);
    spi_result |= (uint32_t)UCAxRXBUF;
	return spi_result;
}
inline void spi_send(uint8_t* data,uint16_t length,uint8_t* buffer = nullptr){
    while(length--){
        UCAxTXBUF = *(data++);
        while (UCAxSTAT & UCBUSY);
        if(buffer){*buffer++ = UCAxTXBUF;}
    }
}
inline void spi_send16(uint16_t* data,uint16_t length){
    while(length--){
        UCAxTXBUF = *data >> 8&0xff; 
        while (UCAxSTAT & UCBUSY);
        UCAxTXBUF = *++data&0xff; 
        while (UCAxSTAT & UCBUSY);
    }
}
#endif
