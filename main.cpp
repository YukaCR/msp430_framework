#ifdef VS_CODE
#define uint32_t unsigned long
#define uint16_t unsigned int
#define uint8_t unsigned char
#endif
#include <msp430.h>
#include <stdint.h>
#include "UCS.h"
#include "TIDriver/ucs.h" 
#define UCBxCTL0 UCB0CTL0
#define UCBxCTL1 UCB0CTL1
#define UCBxBRW UCB0BRW
#define UCBxI2CSA UCB0I2CSA
#define UCBxSTAT UCB0STAT
#define UCBxIFG UCB0IFG
#define UCBxTXBUF UCB0TXBUF
#define max_fail_loop 3000
uint16_t fail_loop=0;
uint8_t fail_count=0;

void i2c_master_init(){
	P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3;
	//P3OUT |= BIT0 + BIT1;
    P3SEL |= BIT0 + BIT1;
	//P3REN |= BIT0 + BIT1;
    
   UCS_setExternalClockSource(32768,4*Mhz);
   UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
   __delay_cycles(1000);
   UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
   UCBxCTL0 = UCMM + UCMST + UCMODE_3 + UCSYNC;
   UCBxCTL1 = UCSSEL_3;
   UCBxBRW = 40u;
}
bool i2c_send_master(uint8_t address,uint8_t cmd,uint8_t data,uint8_t fail_count = 10){
    fail_loop = 0;
    fail_count = 0;
    UCBxIFG = 0x00;
    UCBxI2CSA = address;
    UCBxCTL0 |= UCMST;
    UCBxCTL1 |= UCTR + UCTXSTT ;
    while(!(UCBxIFG & UCTXIFG)){
        fail_loop ++;
        if(fail_loop > max_fail_loop){
            if(fail_count==0){
                return false;
            }
            return i2c_send_master(address,cmd,data,fail_count-1);
        }
    }
    if(UCBxIFG&UCNACKIFG || UCBxIFG&UCALIFG){
        return i2c_send_master(address,cmd,data,fail_count-1);
    }
    fail_loop = 0;
    UCBxTXBUF = cmd;
    while(!(UCBxIFG & UCTXIFG)){
        fail_loop ++;
        if(fail_loop > max_fail_loop){
            return i2c_send_master(address,cmd,data,fail_count-1);
        }
    }
    UCBxIFG = 0x00;
    UCBxCTL1 |= UCTXSTP;
    return true;
}
int main(){
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    i2c_master_init();
    while(1){
    	i2c_send_master(0x3c,0xff,0xff);
    }
}
