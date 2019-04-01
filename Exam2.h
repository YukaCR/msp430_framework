#include "vscode.h"
#include <msp430.h>
#include "Modules/PWM.h"
#include "Modules/Freq+.h"
#include "Modules/ButtonMartix.h"
#include "Modules/oled.h"
#include "profile_int.h"
uint16_t OutputFreq = 1000;
uint16_t PWM1_percent = 50;
uint16_t PWM2_percent = 50;
void Freq_Percent_Srv(double FreqPercent){
    OLED_setXY(1,6);
    oled_print(FreqPercent);
}
void Freq_Srv(double FreqValue){
    OLED_setXY(0,6);
    oled_print(FreqValue);
}
void Key_Srv(uint8_t key){
    switch (key)
    {
        case 0x01+0x10:
            OutputFreq+=100;
            if(OutputFreq>3000)OutputFreq=1000;
            SetPWMFreq(OutputFreq);
            OLED_setXY(3,6);
            oled_print_number(OutputFreq);
            break;
        case 0x02+0x10:
            OutputFreq-=100;
            if(OutputFreq<1000)OutputFreq=3000;
            SetPWMFreq(OutputFreq);
            OLED_setXY(3,6);
            oled_print_number(OutputFreq);
            break;
        case 0x04+0x10:
            SetPWM1Persent(PWM1_percent++);
            OLED_setXY(2,7);
            oled_print_number(PWM1_percent);
            break;
        case 0x08+0x10:
            SetPWM1Persent(PWM1_percent--);
            OLED_setXY(2,7);
            oled_print_number(PWM1_percent);
            break;
        case 0x01 + 0x20:
            SetPWM2Persent(PWM2_percent--);
            OLED_setXY(4,6);
            oled_print(PWM2_percent*3.0/100.0);
            break;
        case 0x02 + 0x20:
            SetPWM2Persent(PWM2_percent--);
            OLED_setXY(4,6);
            oled_print(PWM2_percent*3.0/100.0);
            break;
        default:
            break;
    }
}
int main(){
    FreqCallback = Freq_Srv;
    ButtonCallback = Key_Srv;
    InitPWMPercent(50,50);
    FreqInit();
    SetPWMFreq(OutputFreq);
    OLED_init();
    OLED_setXY(0,0);
    oled_print("Freq:");
    OLED_setXY(1,0);
    oled_print("%:");
    OLED_setXY(2,0);
    oled_print("PWM_%:");
    OLED_setXY(3,0);
    oled_print("PWM_Hz:");
    OLED_setXY(4,0);
    oled_print("DC_V:");
    FreqStart();
    while(1){
        continue;
    };
}
