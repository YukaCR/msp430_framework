/*
*   Author: YukaCR
*   DAC7612U, 12bit 2Ch DAC, 1.25Msps
*   just setup and set value. 
*   can also use as DDS. takes one of three dma. be careful to use.
*/


#ifndef DAC7612U_H
#define DAC7612U_H
//max 10kHz, 500ksps, 50 dots.
//min 100Hz, 500ksps, 5000 dots.
//why not 1Hz, 250kSps, 250000dots, use data compress.
//500, 000 / 4 Bytes at rasp.

#include "DMA_SPI.h"
#include "DDS.h"
#include "SPI.h"
void Setup_DAC7612U_DDS()
{
    P6SEL &= ~BIT5;
    P6DIR |= BIT5;
    P6OUT |= BIT5;
    setup_DMA_SPI();
    ChangeSinProp(1000, 1);
}
void Setup_DAC7612U()
{
    P6SEL &= ~BIT5;
    P6DIR |= BIT5;
    P6OUT |= BIT5;
    Setup_SPI();
}

#if 1
void DAC_Write(uint16_t value,uint16_t head){
    unsigned char send_data_0,send_data_1;
    value|=(head<<12);
    send_data_0 = (value>>8)&0x00ff;
    send_data_1 = value&0x00ff;
    LOADDACS = 0;
    LOADDACS = 1;
    SPI_Write(send_data_0);
    SPI_Write(send_data_1);
    LOADDACS = 0;
    LOADDACS = 1;
}
#endif

#endif
