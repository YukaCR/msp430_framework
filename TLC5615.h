#ifndef __TLC5615_H__
#define __TLC5615_H__
#include "usci_a_spi.h"
void TLC5615_setValue(uint16_t value){
    UCAxSPI_write_data((value<<2)|0xf000);
    //__delay_cycles(8000);
}
#endif