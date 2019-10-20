#if 0
#include "vscode.h"
#include "Modules_Rebuild/SSD1306.h"
int main(){
    Setup_SSD1306();
    SSD1306_Clear();
    GFX_WriteAt("A:Sin B:Squ C:Tri\n", 0, 0);
    GFX_Write("D:Enter #:. *:^Cancel\n");
    GFX_Write("Freq: 1.0 kHz\n");
    GFX_Write("Ampl: 16.384 V\n");
    GFX_InvertRow_Ranged(0, 2 * 6, 5 * 6);
    GFX_InvertRow_Ranged(2, 6 * 6, 127);
    while(1);
}
#endif
#if 1
#include "vscode.h"
#include "Modules_Rebuild/SSD1306_SPI.h"
#include "Modules_Rebuild/Freq++.h"
#include "Modules_Rebuild/to_string.h"
#include "DigitalPowerModules/DPM_Measure_BuiltinADC.h"
DPM_DATA_t DPM_Voltage_Max = 0;
int main(){
    char* resultStr;
    volatile float adc_buffer;
    volatile double result;
    DPM_Setup_Voltage_Measure();
    ADC12IE |= BIT1;
    result = Setup_FreqPP();
    ADC12IE &=~ BIT1;
    Setup_SSD1306();
    SSD1306_Fill(0X00);
    resultStr = to_string(result, 10);
    oled_write_string(resultStr,strlen(resultStr));
    oled_write_char('\n');
    adc_buffer = _Q12toF(DPM_Voltage_Max);
    resultStr = to_string(adc_buffer, 10);
    oled_write_string(resultStr, strlen(resultStr));
    DMA0CTL = 0;
    DMA1CTL = 0;
    WDTCTL = 0XDEAD;
}
#pragma vector = ADC12_VECTOR
interrupt void ADC_ISR(){
    DPM_Voltage_Detect();
    if(DPM_Voltage > DPM_Voltage_Max){
        DPM_Voltage_Max = DPM_Voltage;
    }
    ADC12IFG = 0x00;
}
#endif
#if 0 // OLED Test
int main(){
    Setup_SSD1306();
    SSD1306_Fill(0x00);
    OLED_setXY(0,0);
    oled_write_string("kOKoSuki.", 10);
    LPM4;
    while(1){
        oled_write_char('a');
        __delay_cycles(65534);
    }
    return 0;
}
#endif 
