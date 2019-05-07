#ifndef FP_PID_H
#define FP_PID_H
#include <msp430.h>
#include <vscode.h>
#include "Q15.h"
#include "DMA.h"
#include "MPY32.h"
#include "math.h"
int16_t SetPoint = 250;
int32_t SumError = 0;
#define _Proportion 4.00f
#define _Integral 1.0f
#define _Derivative 0.0f
double px = _Proportion;
double ix = _Integral;
double dx = _Derivative;
int setP(double);
int setI(double);
int setD(double Det);
int32_t Proportion = ceil(_Proportion * 16777216.0f);//Q24 Format 0-128, with 24bit dot and one bit sign.
int32_t Integral = -1 *ceil(_Integral * 16777216.0f);
int32_t Derivative = ceil(_Derivative * 16777216.0f);
int16_t LastError = 0;
int16_t PrevError = 0;
int32_t _PD;
int32_t _ID;
int32_t _DD;
int16_t iError;
void IncPIDInit(){};
int64_t IncPIDCalc(uint16_t NextPoint)
{
    int64_t rec = 0;
    iError = SetPoint - NextPoint;
HW_MPY_32_16_S_SETUP;
    HW_MPY_32_16_S_CONTINUE(Proportion, iError)
    HW_MPY_32_16_S_CONTINUE(Integral, LastError)
    HW_MPY_32_16_S_CONTINUE(Derivative, PrevError)
HW_MPY_32_16_S_END;
    PrevError = LastError;
    LastError = iError;
    rec = RES3;
    rec = rec << 16 | RES2;
    rec = rec << 16 | RES1;
    rec = rec << 16 | RES0;
    return rec>>24;
}
#endif