
#ifndef __SPI_H__
#define __SPI_H__
#include "../TIDriver/usci_a_spi.h"
#include "../TIDriver/ucs.h"
#warning "SPI library use DCOCLK as ACLK source, be careful if some modeules use ACLK at 32768Hz"
#define UCAxTXBUF UCA0TXBUF
#define UCAxSTAT  UCA0STAT
#define UCAxRXBUF UCA0RXBUF 
void SPI_Setup(){
    P3OUT |= BIT3 + BIT4;P3SEL |= BIT3+BIT4;P3REN |= BIT3+BIT4;
    P2OUT |= BIT7; P2SEL |= BIT7; P2REN|= BIT7;
    UCS_initClockSignal(UCS_ACLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    USCI_A_SPI_initMasterParam spi_param;
    spi_param.selectClockSource = USCI_A_SPI_CLOCKSOURCE_ACLK;
    spi_param.msbFirst = USCI_A_SPI_MSB_FIRST;
    spi_param.clockPhase = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    spi_param.clockSourceFrequency = 25 * 1000 * 1000;
    USCI_A_SPI_initMaster(USCI_A0_BASE,&spi_param);
    USCI_A_SPI_enable(USCI_A0_BASE);
};
uint8_t spi_send(uint8_t _data)
{
	UCAxTXBUF = _data; 
	while (UCAxSTAT & UCBUSY);
	return UCAxRXBUF;
}
uint16_t spi_send(uint16_t _data)
{
	UCAxTXBUF = (_data>>8)&0xff; 
	while (UCAxSTAT & UCBUSY);
	UCAxTXBUF = _data&0xff; 
	while (UCAxSTAT & UCBUSY);
	return UCAxRXBUF;
}
uint16_t spi_send(uint32_t _data)
{
	UCAxTXBUF = (_data>>24)&0xff;
	while (UCAxSTAT & UCBUSY);
	UCAxTXBUF = (_data>>16)&0xff;
	while (UCAxSTAT & UCBUSY);
	UCAxTXBUF = (_data>>8)&0xff;
	while (UCAxSTAT & UCBUSY);
	UCAxTXBUF = _data&0xff;
	while (UCAxSTAT & UCBUSY);
	return UCAxRXBUF;
}
inline void UCAxSPI_send_data(uint8_t* data,uint8_t length){
    while(length--){
        UCAxTXBUF = *data++; 
        while (UCAxSTAT & UCBUSY);
    }
}
inline void UCAxSPI_send_data(uint16_t* data,uint8_t length){
    while(length--){
        UCAxTXBUF = *data >> 8&0xff; 
        while (UCAxSTAT & UCBUSY);
        UCAxTXBUF = *++data&0xff; 
        while (UCAxSTAT & UCBUSY);
    }
}
#endif
