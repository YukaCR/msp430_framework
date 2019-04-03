#ifndef __I2C_H__
#define __I2C_H__
#ifdef VS_CODE
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t unsigned char
#endif
#include <msp430.h>
#include <stdint.h>
#include "UCS.h"
#include "TIDriver/ucs.h" 
#define Mhz 1000000L
#define UCBx 0           //UCB0 UCB1
#undef UCBxCTL0
#undef UCBxCTL1
#undef UCBxIFG
#define UCBxCTL0 UCB0CTL0
#define UCBxCTL1 UCB0CTL1
#define UCBxBRW UCB0BRW
#define UCBxI2CSA UCB0I2CSA
#define UCBxSTAT UCB0STAT
#define UCBxIFG UCB0IFG
#define UCBxTXBUF UCB0TXBUF
#define UCBxCTLW0 UCB0CTLW0
#define UCBxRXBUF UCB0RXBUF
#define UCBxI2COA UCB0I2COA
#define UCBxICTL UCB0ICTL
#define max_fail_loop 5
uint16_t fail_loop=0;

void i2c_master_reset();
void i2c_master_init();
bool i2c_start(uint8_t address,int8_t fail_count);
bool i2c_send_data_master(uint8_t data);
void i2c_stop_master();
bool i2c_write_block(uint8_t address,uint8_t cmd,uint8_t* data,uint8_t length,int8_t fail_count);
bool i2c_write_block(uint8_t address,uint8_t cmd,uint8_t data,int8_t fail_count);
bool i2c_write_data(uint8_t address,uint8_t* data,uint8_t length,int8_t fail_count);
bool i2c_write_data(uint8_t address,uint8_t data,int8_t fail_count);

void i2c_master_reset(){
    UCBxCTLW0 = 0x0101;
    UCBxBRW = 0x0000;
    UCBxSTAT = 0X00;
    UCBxRXBUF = 0X00;
    UCBxTXBUF = 0X00;
    UCBxI2COA = 0X00;
    UCBxI2CSA = 0X00;
    UCBxICTL = 0X0000;
    i2c_master_init();
}
void i2c_master_init(){
	P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3;
    #if UCBx == 0
    P3OUT |= BIT0+ BIT1;
    P3SEL |= BIT0 + BIT1;
    P3REN |= BIT0 + BIT1;
    P3DIR = BIT0 + BIT1;
    #endif
   UCS_setExternalClockSource(32768,4*Mhz);
   UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
   __delay_cycles(1000);
   UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
   UCBxCTL0 = UCMM + UCMST + UCMODE_3 + UCSYNC;
   UCBxCTL1 = UCSSEL_3 + UCSWRST;
   UCBxBRW = 40u;
   UCBxCTL1&=~UCSWRST;
}
bool i2c_start(uint8_t address,int8_t fail_count){
	if(fail_count<0){
		return false;
	}
    fail_loop = 0;
    UCBxIFG = 0x00;
    UCBxI2CSA = address;
    UCBxCTL0 |= UCMST;
    UCBxCTL1 |= UCTR + UCTXSTT;
    while(!(UCBxIFG & UCTXIFG)){
        fail_loop ++;
        if(fail_loop > max_fail_loop){
            if(fail_count==0){
                i2c_master_reset();
                return false;
            }
            return i2c_start(address,fail_count-1);
        }
    }
    if(UCBxIFG&UCNACKIFG || UCBxIFG&UCALIFG){
        return i2c_start(address,fail_count-1);
    }
    return true;
}
bool i2c_send_data_master(uint8_t data){
    fail_loop = 0;
    UCBxIFG = 0x00;
    UCBxTXBUF = data;
    while(!(UCBxIFG & UCTXIFG)){
        fail_loop ++;
        if(fail_loop > max_fail_loop){
            i2c_master_reset();
            return false;
        }
    }
    return true;
}
inline void i2c_stop_master(){
    UCBxCTL1 |= UCTXSTP;
}
inline void i2c_release(){
    UCBxCTL1 |= UCSWRST;
}
bool i2c_write_block(uint8_t address,uint8_t cmd,uint8_t data,int8_t fail_count){
	//cooldownDCO();
	UCS_initClockSignal(UCS_MCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);

	if(fail_count<0){
		return false;
	}
	if(!i2c_start(address,fail_count - 1)){
        return false;
    }
    if(!i2c_send_data_master(cmd)){
        return i2c_write_block(address,cmd,data,fail_count - 1);
    }
    if(!i2c_send_data_master(data)){
        return i2c_write_block(address,cmd,data,fail_count - 1);
    };
    i2c_stop_master();
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    return true;
}
bool i2c_write_block(uint8_t address,uint8_t cmd,uint8_t *data,uint8_t length,int8_t fail_count){
	//cooldownDCO();
	UCS_initClockSignal(UCS_MCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);

	if(fail_count<0){
		return false;
	}
	if(!i2c_start(address,fail_count - 1)){
        return false;
    }
    if(!i2c_send_data_master(cmd)){
        return i2c_write_block(address,cmd,data,length,fail_count - 1);
    }
    for(int i = 0; i < length; i ++){
        if(!i2c_send_data_master(data[i])){
            return i2c_write_block(address,cmd,data,length,fail_count - 1);
        };
    }
    i2c_stop_master();
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    return true;
}
bool i2c_write_data(uint8_t address,uint8_t* data,uint8_t length,int8_t fail_count){
	UCS_initClockSignal(UCS_MCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
	if(fail_count<0){
		return false;
	}
	if(!i2c_start(address,fail_count - 1)){
        return false;
    }
    for(int i = 0 ; i < length ; i ++){
        if(!i2c_send_data_master(data[i])){
            return i2c_write_data(address,data,length,fail_count - 1);
        };
    }
    i2c_stop_master();
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    return true;
}
bool i2c_write_data(uint8_t address,uint8_t data,int8_t fail_count){
	UCS_initClockSignal(UCS_MCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
	if(fail_count<0){
		return false;
	}
	if(!i2c_start(address,fail_count - 1)){
        return false;
    }
    if(!i2c_send_data_master(data)){
        return i2c_write_data(address,data,fail_count - 1);
    };
    i2c_stop_master();
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    return true;
}
#endif
