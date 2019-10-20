/*
* Author: YukaCR
* INA260 Power monitor.
* totally failed dute to the shit hw bugs of msp430.
*/
#pragma once
#include "../../vscode.h"
#include "../../TIDriver/usci_b_i2c.h"
const uint8_t INA260_StartCondition[][3] = {
    0x00, 0xff, 0xff,             // reset
    0x00, 0x60, 0xdf, // sample time 1.0ms
    0x06, 0x04, 0x00,       // enable conversion ready flags.
};
uint8_t step = 0;
uint8_t INA260_Result[5] = {0, 0, 0, 0, 0};// *LE-> ALERT, CURR, VOLT
uint16_t data_[2] = {0,0};
uint16_t register_map[8];
void Setup_INA260()
{
    USCI_B_I2C_initMasterParam i2c_param;
    P3REN|=BIT0+BIT1;
    P3DIR|=BIT0+BIT1;
    P3OUT|=BIT0+BIT1;
    P3SEL|=BIT0+BIT1;
    i2c_param.dataRate = USCI_B_I2C_SET_DATA_RATE_400KBPS;
    i2c_param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_ACLK;
    i2c_param.i2cClk = 4000000;
    USCI_B_I2C_initMaster(USCI_B0_BASE, &i2c_param);
    USCI_B_I2C_setMode(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_MODE);
    UCB0I2CSA = 64;
    UCB0CTL1 &=~ UCSWRST;
        UCB0IFG = 0x00;

    for (uint8_t i = 0; i < 3; i++)
    {
        UCB0CTL1 &=~ UCSWRST;
        UCB0CTL1 |= UCTXSTT ;
        UCB0TXBUF = INA260_StartCondition[i][0];
        while (!(UCB0IFG & UCTXIFG))
            ;
        UCB0IFG = 0x00;
        UCB0TXBUF = INA260_StartCondition[i][1];
        while (!(UCB0IFG & UCTXIFG))
            ;
        UCB0IFG = 0x00;
        UCB0TXBUF = INA260_StartCondition[i][2];
        while (!(UCB0IFG & UCTXIFG))
            ;
        UCB0IFG = 0x00;
        UCB0CTL1 |= UCTXSTP ;
    }

    // for debug, read significant register.
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTT | UCTR;
    __delay_cycles(10);
    UCB0TXBUF = 0xfe;
    while(!(UCB0IFG & UCTXIFG));
    UCB0IFG = 0x00;
    UCB0CTL1 |= UCSWRST;
    
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTT;
    while(!(UCB0IFG & UCRXIFG));
    UCB0IFG = 0x00;
    data_[0] = (uint16_t)UCB0RXBUF << 8;
    while (UCB0CTL1 & UCTXSTP);
    UCB0IFG = 0x00;
    data_[0] |= UCB0RXBUF;
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTP;

    // already.
    step = 5;
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTT | UCTR;
    UCB0TXBUF = 0x01;               // voltage address.
    while (!(UCB0IFG & UCTXIFG));   // pre addressing
        UCB0IFG = 0x00;
    UCB0CTL1 |= UCSSEL__ACLK | UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);

    // P2IE |= BIT6;
    //UCB0IFG &= ~UCTXIFG;
    //UCB0IE |= UCTXIE + UCRXIE;
}
#pragma vector = PORT2_VECTOR
interrupt void P2_ISR()
{
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTT; // START RECV
    __no_operation();                  // debug
}
#pragma vector = USCI_B0_VECTOR
interrupt void I2C_ISR()
{
    switch (__even_in_range(UCB0IV, 0x0c))
    {
    case 0x0a: //ucrxifg
        break;
    case 0x0c: //uctxifg
        UCB0CTL1 |= UCSWRST;
        UCB0CTL1 = UCSSEL__ACLK | UCTXSTT;            // just receive, it's ok.
        break;
    default:
        break;
    }
}
