//USCI_A_API
#include <msp430.h>
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
void UCAxSPI_master_init(void msb_first,uint32_t speed,void raise){
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    #if KPH
    UCAxCTL0 = UCCKPH + raise?0X00:UCCKPL + msb_first?UCMSB:0X00 + UCMST + UCMODE_2 + UCSYNC;
    #else
    UCAxCTL0 = raise?UCCKPL:0x00 + msb_first?UCMSB:0X00 + UCMST + UCMODE_2 + UCSYNC;
    #endif
    UCAxCTL1 = UCSSEL_3 + UCSWRST;
    UCAxBRW = (4 * Mhz) / speed;
    UCAxCTL1 &=~ UCSWRST;
    msbfirst = msb_first;
}

void UCAxSPI_write_data(uint8_t data){
    if(SS_LOW_ENABLE){
        SS_PORT &=~ SS_PIN;
    }
    else{
        SS_PORT |= SS_PIN;
    }
    UCAxTXBUF = data;
    while(!(UCAxIFG & UCTXIFG));
    if(SS_LOW_ENABLE){
        SS_PORT |= SS_PIN;
    }
    else{
        SS_PORT &=~ SS_PIN;
    }
}
void UCAxSPI_write_data(uint8_t* data,uint8_t length){
    for(uint8_t i = 0; i < length; i ++){
        UCAxSPI_write_data(data[i]);
    }
}
void UCAxSPI_write_data(uint16_t data){
    if(SS_LOW_ENABLE){
        SS_PORT &=~ SS_PIN;
    }
    else{
        SS_PORT |= SS_PIN;
    }
    if(msbfirst){
        UCAxTXBUF = (data&0xff00)>> 8;
        while(!(UCAxIFG & UCTXIFG));
        UCAxTXBUF = data&0x00ff;
        while(!(UCAxIFG & UCTXIFG));
    }
    else{
        UCAxTXBUF = data&0x00ff;
        while(!(UCAxIFG & UCTXIFG));
        UCAxTXBUF = (data&0xff00)>> 8;
        while(!(UCAxIFG & UCTXIFG));
    }
    if(SS_LOW_ENABLE){
        SS_PORT |= SS_PIN;
    }
    else{
        SS_PORT &=~ SS_PIN;
    }
}
void UCAxSPI_write_data(uint16_t* data,uint8_t length){
    for(uint8_t i = 0; i < length; i ++){
        UCAxSPI_write_data(data[i]);
    }
}


int main(){
    uint16_t i = 0;
    uint16_t data;
    WDTCTL = WDTPW | WDTHOLD;
    setupDCO();
    UCAxSPI_master_init(true,1.21*Mhz,true);
	P5SEL |= BIT4 | BIT5; 
	P5SEL |= BIT2 | BIT3; 
    P3SEL |= BIT3 + BIT4;
    P2SEL |= BIT7;
    P3DIR |= BIT3;
    P6DIR |= BIT6;
    P6OUT |= BIT6;
    while(1){
        for(i = 0 ; i < 1024; i ++){
            data = (i<<2)| 0xf000;
            UCAxSPI_write_data(data);
            __delay_cycles(3000);
        }
    }
}
