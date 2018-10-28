//USCI_A_API
#ifndef __USCI_A_SPI_H__
#define __USCI_A_SPI_H__
#include <msp430.h>
#include <math.h>
#include "ucs.h"
#include "TIDriver/ucs.h"
#define Mhz 1000000L
#define Khz 1000L
#define UCAx 0
#define KPH 1
#define SS_PORT P6OUT
#define SS_PIN BIT6
#define UCAxCTL0 UCA0CTL0
#define UCAxCTL1 UCA0CTL1
#define UCAxBRW UCA0BRW
#define UCAxTXBUF UCA0TXBUF
#define UCAxRXBUF UCA0RXBUF
#define UCAxSTAT UCA0STAT
#define UCAxIFG UCA0IFG
//KPH KPL DATA1
// 0   0  Send on raise. first and last bit lost
// 0   1  Send on falldown. data not prepare ok.
// 1   0  Send on raise. looks great.
// 1   1  Send on falldown. looks great.
//raise send : 1 0 or 0 1
//falldown send: 0 0 or 1 1
//recommand KPH = 1
bool SS_LOW_ENABLE = true;
bool msbfirst;
uint8_t spi_send(const uint8_t _data)
{
	UCAxTXBUF = _data; // setting TXBUF clears the TXIFG flag
	while (UCAxSTAT & UCBUSY)
		; // wait for SPI TX/RX to finish

	return UCAxRXBUF; // reading clears RXIFG flag
}
void UCAxSPI_master_init(bool msb_first,uint32_t speed,bool raise){
	P5SEL |= BIT4 | BIT5; 
	P5SEL |= BIT2 | BIT3; 
    P3SEL |= BIT3 + BIT4;
    P2SEL |= BIT7;
    P3DIR |= BIT3;
    P2DIR |= BIT7;
    P6DIR |= BIT6;
    P6OUT |= BIT6;
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCAxCTL1 |= UCSWRST;
    #if KPH
    UCAxCTL0 = UCCKPH + (raise?0X00:UCCKPL) + (msb_first?UCMSB:0X00) + UCMST + UCMODE_0 + UCSYNC;
    #else
    UCAxCTL0 = raise?UCCKPL:0x00 + msb_first?UCMSB:0X00 + UCMST + UCMODE_0 + UCSYNC;
    #endif
    UCAxCTL1 = UCSSEL_3 + UCSWRST;
    UCAxBRW = (uint16_t)ceil((double)(4 * Mhz) / (double)speed);
    UCAxCTL1 &=~ UCSWRST;
    msbfirst = msb_first;
}
void spi_ss_enable(){
    if(SS_LOW_ENABLE){
        SS_PORT &=~ SS_PIN;
    }
    else{
        SS_PORT |= SS_PIN;
    }
}
void spi_ss_disable(){
    if(SS_LOW_ENABLE){
        SS_PORT |= SS_PIN;
    }
    else{
        SS_PORT &=~ SS_PIN;
    }
}
void UCAxSPI_write_data(uint8_t data){
    spi_ss_enable();
    spi_send(data);
    spi_ss_disable();
}
void UCAxSPI_write_data(uint8_t* data,uint8_t length){
    spi_ss_enable();
    for(uint8_t i = 0; i < length; i ++){
        spi_send(data[i]);
    }
    spi_ss_disable();
}
void UCAxSPI_write_data(uint16_t data){
    spi_ss_enable();
    if(msbfirst){
        spi_send(data>>8);
        spi_send(data&0xff);
    }
    else{
        spi_send(data&0xff);
        spi_send(data>>8);
    }
    spi_ss_disable();
}
void UCAxSPI_write_data(uint16_t* data,uint8_t length){
    spi_ss_enable();
    for(uint8_t i = 0; i < length; i ++){
        spi_send(data[i]);
    }
    spi_ss_disable();
}
/* USCI_A_SPI_TestOK;
int main(){
    uint16_t i = 0;
    setupDCO();
    UCAxSPI_master_init(true,1.21*Mhz,true);
    while(1){
        for(i = 0 ; i < 512; i ++){
            UCAxSPI_write_data((i<<2)|0xf000);
            __delay_cycles(8000);
        }
    }
}
*/
#endif