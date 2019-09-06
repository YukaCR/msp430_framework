#pragma once
#include "../vscode.h"
#include "../IQMath/QmathLib.h"
#include "msp430.h"
void Servo_Setup(){
    P2SEL |= BIT0;
    P2DIR |= BIT0 ;
    P2DS |= BIT0 ;
    TA1CTL = TASSEL__ACLK | ID__4 | MC__STOP;
    TA1CCR0 = 20000;
    TA1CCR1 = 1500;
    TA1CCTL0 = CM_1 | SCS | SCCI;
    TA1CCTL1 = CM_0 | SCS | OUTMOD_2;
    TA1CTL |= MC__UP | TACLR;
}

void Servo_SetAngle(uint16_t angle){// 0-180
    TA1CCR1 = 530 + _Q11mpy(angle,_Q11(10.944444444444445));
}