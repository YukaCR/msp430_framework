#include "DPM_PWMControl.h"

void DPM_Setup_PWMControl(){
    //Timer A2 Init
    P2SEL |= BIT2 | BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT3 + BIT4;
    P5SEL |= BIT4 | BIT5 | BIT2 | BIT3;
}
