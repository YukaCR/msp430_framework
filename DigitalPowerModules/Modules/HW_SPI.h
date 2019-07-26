#ifndef TFT
#define TFT
#include "../../vscode.h"
#include <msp430.h>
#include "../../TIDriver/usci_a_spi.h"
//tft : !( cpol + cpha )
inline void SPI_SetCPOL_CPHA(uint8_t CPOL, uint8_t CPHA){
    UCA0CTL0 &=~ UCCKPH | UCCKPL;
    UCA0CTL0 |= (CPHA << 6) | (CPOL << 5);
}
void Setup_SPI_Master(){
    P3OUT |= BIT3 + BIT4;P3SEL |= BIT3+BIT4;P3REN |= BIT3+BIT4;
    P2OUT |= BIT7; P2SEL |= BIT7; P2REN|= BIT7;
    USCI_A_SPI_initMasterParam spi_param;
    spi_param.selectClockSource = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    spi_param.msbFirst = USCI_A_SPI_MSB_FIRST;
    //spi_param.clockPhase = USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    spi_param.clockPhase = USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    spi_param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    spi_param.clockSourceFrequency = 4000000;
    spi_param.desiredSpiClock = 1000000;
    USCI_A_SPI_initMaster(USCI_A0_BASE,&spi_param);
    USCI_A_SPI_enable(USCI_A0_BASE);
}
inline uint8_t spi_send(uint8_t _data)
{
	UCA0TXBUF = _data; 
	while (UCA0STAT & UCBUSY);
	return UCA0RXBUF;
}
inline uint16_t spi_send16(uint16_t _data){
    register uint16_t spi_result;
	UCA0TXBUF = (_data>>8)&0xff; 
	while (UCA0STAT & UCBUSY);
    spi_result = (uint16_t)UCA0RXBUF << 8;
	UCA0TXBUF = _data&0xff; 
	while (UCA0STAT & UCBUSY);
    spi_result |= UCA0RXBUF;
	return spi_result;
}
#endif
