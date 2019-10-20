/*
*   Author: YukaCR
*   sukoi no hwmpy initilaize serial and calculation.
*   can also use dma to notify source. (?
*/
#ifndef MPY32_H
#define MPY32_H
#include <msp430.h>
#include "../vscode.h"
bool MPY_BUSY = 0;
uint8_t MPY_STATUS = 0;
uint64_t MPY32_RESULT = 0X00;
#include "DMA.h"
#define HW_MPY_16_16_STA_U_DMA(_OP1,_OP2,_Result) MPY = _OP1;\
        OP2 = _OP2;\
        MPY32CTL0 = MPYSAT;\
        _Result = MPY32_RESULT;
#define HW_MPY_16_16_STA_U(_OP1,_OP2,_Result) MPY = _OP1;\
        OP2 = _OP2;\
        MPY32CTL0 = MPYSAT;\
        _Result = *(uint32_t*)&RES0;
#define HW_MPY_16_16_STA_U_RShift(_OP1,_OP2,_Result,_Bit) MPY = _OP1;\
        OP2 = _OP2;\
        MPY32CTL0 = MPYSAT;\
        __delay_cycles(5);\
        _Result = (*(uint32_t*)&RES0) >> _Bit;
#define HW_MPY_32_16_S_SETUP MPY32CTL0 = MPYM__MACS + OP1_32;RES0=0;RES1=0;RES2=0;RES3=0;
#define HW_MPY_32_16_S_CONTINUE(_OP1,_OP2) MACS32L = _OP1&0X0000FFFF;\
    MACS32H = (_OP1 >> 16);\
    OP2 = _OP2;\
    __delay_cycles(21);
#define HW_MPY_32_16_S_END MPY32CTL0 = MPYSAT;
#define HW_MPY_16_16_S_SETUP MPY32CTL0 = MPYM__MACS;RES0=0;RES1=0;RES2=0;RES3=0;
#define HW_MPY_16_16_S_CONTINUE(_OP1,_OP2) MACS = _OP1;\
    OP2 = _OP2;
#define HW_MPY_16_16_S_END MPY32CTL0 = MPYSAT;
#define HW_MPY_16_32_U(_OP1,_OP2,_Result) MPY = _OP1;\
        OP2L = _OP2&0XFFFF;\
        OP2H = _OP2>>16;\
        __delay_cycles(10);\
        _Result = *((uint64_t*)(&RES0));
#define HW_MPY_32_32_U(_OP1,_OP2,_Result) MPY_L = _OP1&0xffff;\
        MPY_H = _OP1>>16;\
        OP2L = _OP2&0xffff;\
        OP2H = _OP2>>16;\
        __delay_cycles(21);\
        _Result = *((uint64_t*)(&RES0));
#endif
