
#include "../../TIDriver/usci_b_i2c.h"
#include "../../TIDriver/ucs.h"

bool scan_result[128] = {0};

bool I2C_scan_read(){
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    bool find_device = false;
    USCI_B_I2C_initMasterParam i2c_param;
    //P3DIR|=BIT0+BIT1;
    P3REN|=BIT0+BIT1;
    P3SEL|=BIT0+BIT1;
    i2c_param.dataRate = USCI_B_I2C_SET_DATA_RATE_100KBPS;
    i2c_param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    i2c_param.i2cClk = 4000000;
    USCI_B_I2C_initMaster(USCI_B0_BASE, &i2c_param);
    //USCI_B_I2C_setMode(USCI_B0_BASE,USCI_B_I2C_TRANSMIT_MODE);

    for(uint8_t x = 0; x < 128 ; x++){
        uint8_t result;
        UCB0I2CSA = x;
        UCB0CTL1 &=~ UCSWRST;
        UCB0CTL1 |= UCTXSTT;
        while (UCB0CTL1 & UCTXSTT);
        while(!(result = UCB0IFG));
        if(result & UCRXIFG){
            scan_result[x] = true;
            find_device = true;
        }
        UCB0IFG = 0;
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);
        UCB0CTL1 |= UCSWRST;
    }
    return find_device;
}
bool I2C_scan_write(){
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    bool find_device = false;
    USCI_B_I2C_initMasterParam i2c_param;
    P3DIR|=BIT0+BIT1;
    P3OUT|=BIT0+BIT1;
    P3REN|=BIT0+BIT1;
    P3SEL|=BIT0+BIT1;
    i2c_param.dataRate = USCI_B_I2C_SET_DATA_RATE_100KBPS;
    i2c_param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    i2c_param.i2cClk = 4000000;
    USCI_B_I2C_initMaster(USCI_B0_BASE, &i2c_param);
    USCI_B_I2C_setMode(USCI_B0_BASE,USCI_B_I2C_TRANSMIT_MODE);

    for(uint8_t x = 0; x < 128 ; x++){
        volatile uint8_t result;
        UCB0I2CSA = x;
        UCB0CTL1 &=~ UCSWRST;
        UCB0CTL1 |= UCTXSTT;
        UCB0TXBUF = 0XFF;
        while(!(result = UCB0IFG));
        if(result & UCTXIFG){
            scan_result[x] = true;
            find_device = true;
        }
        UCB0IFG = 0;
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);
        UCB0CTL1 |= UCSWRST;
    }
    return find_device;
}
