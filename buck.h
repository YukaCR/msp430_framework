#ifndef BUCK_H
#define BUCK_H
#include <msp430.h> 
#include "vscode.h"
#include "stdint.h"
#include "Modules/adc_sch.h"
#include "Modules/PWM.h"
#include "profile_int.h"

#if 0
int main(){
    P1DIR |= BIT1;                            // P1.1 output

    P1DIR |= BIT0;                            // ACLK set out to pins
    P1SEL |= BIT0;
    P2DIR |= BIT2;                            // SMCLK set out to pins
    P2SEL |= BIT2;
    P7DIR |= BIT7;                            // MCLK set out to pins
    P7SEL |= BIT7;
    burnDCO();
    InitPWM(50,50);
    ADC12_init();
    while(1){
        continue;
    }
}
#endif
#endif
