#if 0
__enable_interrupt();
WDTCTL = WDTPW | WDTHOLD;
uint8_t i = 180;
Setup_DPM();
Servo_Setup();
// DPM_PWMC_Setup_PWMControl();
//Setup_Motor();
#endif
/* Servo Test */
#if 0
while(1){
    if(i==30){
        i = 150;
    }
    if(TA1CTL & TAIFG){
        Servo_SetAngle(i--);
        TA1CTL &=~ TAIFG;
    }
}
#endif
#if 0
    while (1)
    {
        // BIT4 BIT3 BIT2 BIT1
        if (TA2CTL & TAIFG)
        {
            uint8_t P1STA = ~(P1IN & (BIT2 | BIT3 | BIT4 | BIT5));
            //P1STA = ~P1STA;
            TA2CTL &=~ TAIFG;
            if (P1STA & BIT5)
            {
                TurnRight();
                //check point?
            }
            else if (P1STA & BIT2)
            {
                TurnLeft();
                // check point?
            }
            else if ((P1STA & BIT4)||(P1STA & BIT3))
            {
                NormalRun();
                //can't be check point
            }
            else if(~P1STA){
                TA2CCR2 = 0;
                TA2CCR1 = 0;
            }
            else
            {
                /* code */
            }
        }
    }
    while (P2IN & BIT3)
        ; // ???????
#endif