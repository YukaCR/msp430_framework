/*
 * motor.c
 *
 *  Created on: 2014-7-15
 *      Author: dean
 */

#include <msp430.h>
#include "motor.h"
void DCmotor(int p)
{
  switch(p)
  {
  case 0:					//ͣת
    {
      P1OUT &=~ BIT5;
      P2OUT &=~ BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 1:					//��ת
    {
      P1OUT |= BIT5;
      P2OUT |= BIT5;
      P2OUT &=~ BIT4;
      break;
    }
  case 2:					//��ת
    {
      P1OUT |= BIT5;
      P2OUT &=~ BIT5;
      P2OUT |= BIT4;
      break;
    }
  }
}

