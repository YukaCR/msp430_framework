// simple hw I2C driver.
// not recommand use the USCIBxI2C mode. 
// you'd better use the software i2c because of the shit hw bug.
#ifndef IIC_H
#define IIC_H
#include "msp430.h"
#include "stdint.h"
#include "../TIDriver/usci_b_i2c.h"
inline void IIC_begin(uint8_t Address){
    P3SEL|=BIT0+BIT1;P3DIR|=BIT0+BIT1;
    P3REN|=BIT0+BIT1;P3OUT|=BIT0+BIT1;
    USCI_B_I2C_initMasterParam i2c_param;
    i2c_param.dataRate =USCI_B_I2C_SET_DATA_RATE_400KBPS;
    i2c_param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    i2c_param.i2cClk = 4000000;
    USCI_B_I2C_initMaster(USCI_B0_BASE, &i2c_param);
    USCI_B_I2C_setMode(USCI_B0_BASE,USCI_B_I2C_TRANSMIT_MODE);
    USCI_B_I2C_setSlaveAddress(USCI_B0_BASE,Address);
    USCI_B_I2C_enable(USCI_B0_BASE);

}
inline void IIC_Start(){
    
    if(!UCB0CTL0 & UCSWRST){
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP); 
        UCB0CTL0 |= UCSWRST;
    }
    UCB0CTL0 &=~ UCSWRST;
    UCB0IFG = 0X00;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0CTL1 & UCTXSTT);
}
inline void IIC_Send(uint8_t Data){
    UCB0TXBUF = Data;
    while(!UCB0IFG & UCTXIFG);
}
inline void IIC_Stop(){
    UCB0CTL1 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP); 
    UCB0CTL0 |= UCSWRST;
}
#endif