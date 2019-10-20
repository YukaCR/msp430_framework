/*
*   Author: YukaCR
*   INA260 SPI warpper for msp430.
*   use stc8f2k60s2 as a protocol conversion intermediary.
*/
#include "vscode.h"
#include "HW_SPI.h"
volatile uint8_t INA260_Data[4];
bool INA260_Ready  = false;
void INA260_Setup(){
    Setup_SPI_Master();
    P7REN |= BIT0;
    P7DS  |= BIT0;
    P7DIR |= BIT0;
    P7OUT |= BIT0;
    P2IE |= BIT6;
}
#pragma vector = PORT2_VECTOR
interrupt void P2_ISR(){
    P2IFG = 0x00;
    P2IE &=~ BIT6;
    if(INA260_Ready){
        return;
    }
    UCA0BRW = 14;
    SPI_SetCPOL_CPHA(false,true);
    P7OUT &=~ BIT0;
    INA260_Data[0] = spi_send(0x88);
    __delay_cycles(40);
    INA260_Data[1] = spi_send(0xf1);
    __delay_cycles(40);
    INA260_Data[2] = spi_send(0xf2);
    __delay_cycles(40);
    INA260_Data[3] = spi_send(0xa0);
    P7OUT |= BIT0;
    INA260_Ready = true;
}
