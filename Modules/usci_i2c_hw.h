#ifndef __I2C_HW_H__
#define __I2C_HW_H__
#include <msp430.h>
#include "../vscode.h"
char count;
bool NC = false;
bool error = false;
uint8_t *tran_buf;
void i2c_send_init(uint8_t slave_address, uint16_t prescale = 12)
{
    __enable_interrupt();
    P3OUT |= BIT0+ BIT1;
    P3SEL |= BIT0 + BIT1;
    P3REN |= BIT0 + BIT1;
    P3DIR |= BIT0 + BIT1;
    UCB0CTL1 = UCSWRST;                   // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;        // Use SMCLK, keep SW reset
    UCB0BRW = prescale;                   // set prescaler
    UCB0I2CSA = slave_address;            // Set slave address
    UCB0CTL1 &= ~UCSWRST;
    UCB0IE = UCNACKIE + UCTXIE;
}
void i2c_send(uint8_t bcount, uint8_t *field)
{
    tran_buf = field;
    NC = true;
    count = bcount;
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
}
#pragma vector = USCI_B0_VECTOR
interrupt void USCIB0_ISR()
{
    switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;                           // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10: break;                           // Vector 10: RXIFG
  case 12:                                  // Vector 12: TXIFG  
    if (count)                          // Check TX byte counter
    {
      UCB0TXBUF = *(tran_buf);                   // Load TX buffer
      tran_buf ++;
      count--;                          // Decrement TX byte counter
    }
    else
    {
      UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
      UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
      NC = 0;
    }
    break;
  default: break;
  }
}
#endif

