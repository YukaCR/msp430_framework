#include "vscode.h"
#include <msp430.h>
#include "Modules/Freq+.h"
#include "Modules/PWM.h"
#include "Modules/ButtonMartix.h"
#include "Modules/adc_sch.h"
#include "Modules/1602.h"
#include "profile_int.h"
int8_t PWMPercent = 50;
int8_t DCPercent = 50;
int32_t PWMFreq_ = 1000;
double Div = 2.0;
inline void ButtonISR(uint8_t key)
{
    switch (key)
    {
    case 0x01 + 0x10:
        SetPWM2Persent(++PWMPercent);
        (PWMPercent >= 100) && (PWMPercent = 0);
        break;
    case 0X02 + 0X10:
        SetPWM2Persent(--PWMPercent);
        (PWMPercent <= 0) && (PWMPercent = 100);
        break;
    case 0X04 + 0X10:
        PWMFreq_ += 100;
        SetPWMFreq(PWMFreq_);
        (PWMFreq_ >= 3000) && (PWMFreq_ = 1000);
        break;
    case 0X08 + 0X10:
        PWMFreq_ -= 100;
        SetPWMFreq(PWMFreq_);
        (PWMFreq_ <= 1000) && (PWMFreq_ = 3000);
    case 0x01 + 0x20:
        SetPWM1Persent(++DCPercent);
        (DCPercent >= 100) && (DCPercent = 0);
        break;
    case 0x02 + 0x20:
        SetPWM1Persent(--DCPercent);
        (DCPercent <= 0) && (DCPercent = 100);
        break;
    default:
        break;
    }
}
inline void FreqISR()
{
    uint32_t _data = 0;
    char dstr[10];
    int8_t count = 0;
    _data += floor(FreqResult);
    clear();
    home();
    while(_data){
        dstr[count++] = (char)((_data%10)+48);
        _data /= 10;
    }
    while(count){
        printChar(dstr[--count]);
    }
    printStr("Hz");
    _data = floor(Freq_Percent);
    printInt(_data);
    printChar('%');
}
inline void VPPISR()
{
    double data = 3.27 * Div * (double)(V_max - V_min) / 4096.0;
    V_max = 0;
    V_min = 0;
    setCursor(0,1);
    printFloat(data, 4);
    printChar('V');
}
int main()
{
    InitPWMPercent(50, 50);
    FreqInit();
    __init__();
    init();
    home();
    Button_Martix_Init();
    ADC12_init();
    FreqStart();
    while (1)
    {
        if (Freq_Flag)
        {
            FreqISR();
            VPPISR();
            Freq_Flag = 0;
            Freq_Percent_Flag = 0;
            FreqStart();
        }
        if (Button_Flag)
        {
            ButtonISR(KeyValue);
            Button_Flag = 0;
        }
    }
}
