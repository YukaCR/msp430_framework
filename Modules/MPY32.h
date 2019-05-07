#ifndef MPY32_H
#define MPY32_H
#include <msp430.h>
#include "vscode.h"
bool MPY_BUSY = 0;
uint8_t MPY_STATUS = 0;
uint64_t MPY32_RESULT = 0X00;
#include "DMA.h"
#define HW_MPY_16_16_STA_U(_OP1,_OP2,_Result) MPY = _OP1;\
        OP2 = _OP2;\
        MPY32CTL0 = MPYSAT;\
        _Result = MPY32_RESULT;
#define HW_MPY_32_16_S_SETUP MPY32CTL0 = MPYM__MACS + OP1_32;RES0=0;RES1=0;RES2=0;RES3=0;
#define HW_MPY_32_16_S_CONTINUE(_OP1,_OP2) MACS32L = _OP1&0X0000FFFF;\
    MACS32H = (_OP1 >> 16);\
    OP2 = _OP2;\
    __delay_cycles(21);
#define HW_MPY_32_16_S_END MPY32CTL0 = MPYSAT;
#endif