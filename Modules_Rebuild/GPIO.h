// try to code like stm32. competely failed.
#ifndef GPIO_H
#define GPIO_H
#include "msp430.h"
#include "stdint.h"
struct GPIO_Struct{
    volatile uint8_t* IDR;
    volatile uint8_t* ODR;
    volatile uint8_t* DR;
    volatile uint8_t* REN;
    volatile uint8_t* STR;
    volatile uint8_t* SR;
    volatile uint8_t* IE;
    volatile uint8_t* IES;
    volatile uint8_t* IFG;
    volatile uint16_t* IV;
};
const GPIO_Struct P1={&P1IN,&P1OUT,&P1DIR,&P1REN,&P1DS,&P1SEL,&P1IE,&P1IES,&P1IFG,&P1IV};
const GPIO_Struct P2={&P2IN,&P2OUT,&P2DIR,&P2REN,&P2DS,&P2SEL,&P2IE,&P2IES,&P1IFG,&P1IV};
const GPIO_Struct P3={&P3IN,&P3OUT,&P3DIR,&P3REN,&P3DS,&P3SEL,0X00,0x00,0x00,0x00};
const GPIO_Struct P4={&P4IN,&P4OUT,&P4DIR,&P4REN,&P4DS,&P4SEL,0X00,0x00,0x00,0x00};

#endif