#if 0
#include <msp430.h>
#include "vscode.h"
#include "math.h"
#include "Modules/PWM_Burn.h"
#include "IQMath/QmathLib.h"
#include "Modules_Rebuild/ADC_EV_DMA.h"
#include "Modules_Rebuild/SPWM_int.h"
#include "Modules_Rebuild/Nokia5110.h"
#include "Modules/ButtonMartix.h"
#define Cac 5
#include "IQMath/QmathLib.h"
//status regs
uint32_t ADC_EV_x_Result = 0;
uint16_t ADC_Count = 0;
bool DMA_ISR_SEND = 0;
uint8_t avg = Cac;
float avg_f = 0;
float avg_v = 0;
uint8_t AVG_RP_COUNT = 0;
bool RMS_V_Meas = 0;

uint8_t current_position = 0;
bool Current_Selected_isAmpl = 0;
bool dot_enabled = 0;
uint8_t dot_position = 0;
#define TB_C_MAX 3
uint8_t TB_C = 0;


//current sense
int32_t ModRx = 0;
//display layout
/*
*  Current:     mA 
*  Voltage:      V
*  Freqency:    Hz
*  \InputArea
*  NumberPad: input
*  A,B: Step in/out
*  C,D 
*  *: Enter
*  #: Switch/Cancel
*/

//display registers
enum CurrentSelect_PT{
    Current=0, Voltage, Freqency
}Current_Select;
float RMS_I;
uint16_t RMS_V=0XFFFF;
uint16_t Freq;
bool input_status = 0;
uint32_t input_buffer = 0;

uint16_t ADC12MAX  = 0;

int8_t FrequencyStep[2]=  {-1,1};
void Update_UI();

uint16_t settingCurrent = 1000  / 1.1;
uint16_t sc_ov = 20;
float CurrentModulation = 1;
int main()
{
    //Setup_Nokia5110();
    //GFX_Fill(0x00);
    Setup_SPWM();
    InitPWM();
    //ADC12_EV_init();
    //Setup_Button_Martix();
    TA0CTL |= TAIE;
    //GFX_Write("Current\n            mAVoltage\n             VFreqency    Hz\n");
    //Update_UI();
    //_Q15imag(0xff,0xff);
    SPWM_Freq(50);
    while (1)
    {};
#if 0
        if (DMA_ISR_SEND) //EV Calaculation
        {
            float EV_Data = 0;
            /*ADC_EV_x_Result *= 4;
            ModRx *= 3;
            EV_Data += ADC_EV_x_Result - ModRx;
            EV_Data /= ADC_Count;
            EV_Data = sqrtf(EV_Data);
            EV_Data /= 4096;
            EV_Data *= 3.3;*/
            EV_Data = (float)ADC12MAX * 3.3  / 4096.0;
            if(RMS_V_Meas){
                avg_v = EV_Data;
                ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_3;
                RMS_V_Meas =0;
                AVG_RP_COUNT = 0;
                GFX_WriteAt(avg_v,3,6);
                if(Current_Select == Voltage){
                    GFX_InvertRow_Ranged(3,6,78);
                }
                ADC12CTL0 |= ADC12ENC;
                ADC12CTL0 |= ADC12SC;
                continue;
            }
            if (avg--)
            {
                avg_f += EV_Data;
            }
            else
            {
                avg_f += EV_Data;
                avg = Cac;
                RMS_I = avg_f / (float)(Cac + 1);
                GFX_ClearRow_Ranged(1,6,72);
                GFX_WriteAt(RMS_I*1100, 1, 6);
                if(Current_Select == Current){
                    GFX_InvertRow_Ranged(1,6,72);
                }
                avg_f = 0;
                if(AVG_RP_COUNT > 30){
                    RMS_V_Meas = true;
                    ADC12MCTL0 = ADC12SREF_0 + ADC12INCH_2;
                }
            }
            if(!(ADC12MAX < settingCurrent + sc_ov && ADC12MAX > settingCurrent - sc_ov)){
                if(ADC12MAX < settingCurrent + sc_ov){
                    SPWM_SetAmp(currentAmp+1);
                }
                else{
                    SPWM_SetAmp(currentAmp-1);
                }
            }
            ADC_Count = 0;
            ADC_EV_x_Result = 0;
            ModRx = 0;
            DMA_ISR_SEND = 0;
            ADC12MAX = 0;
            ADC12CTL0 |= ADC12ENC;
            ADC12CTL0 |= ADC12SC;
        }
        if (Button_Flag )
        {
            if(TB_C -TB_C_MAX){
                Button_Flag = 0;
                continue;
            }
            TA0CTL |= TACLR;
            TB_C= 0;
            Button_Flag = 0;
            if (KeyCode >= '0' && KeyCode <= '9')
            {
                if (!input_status)
                {
                    input_status = 1;
                }
                input_buffer = input_buffer * 10 + KeyCode - 48;
                current_position++;
            }
            else
            {
                switch (KeyCode)
                {
                case '*'://Switch Cancel
                    if (input_status)
                    {
                        input_status = 0;
                        input_buffer = 0;
                        current_position = 0;
                        dot_enabled = 0;
                        dot_position = 0;
                    }
                    else{
                        if(Current_Select==Freqency){
                            Current_Select = Current;
                        }
                        else{
                            Current_Select = (CurrentSelect_PT)(Current_Select+ 1);
                        }
                    }
                    break;
                case '#'://Enter
                    switch(Current_Select){
                        case Current:
                            settingCurrent = input_buffer / 1.1;
                            break;//?????
                        case Voltage:
                            break;
                        case Freqency:
                            SPWM_Freq(input_buffer);
                            break;
                    }
                    input_status = 0;
                    input_buffer = 0;
                    current_position = 0;
                    dot_enabled = 0;
                    dot_position = 0;
                    break;
                case 'A':
                    switch(Current_Select){
                        case Current:
                            settingCurrent += 50;
                            break;//?????
                        case Voltage:
                            break;//?????
                        case Freqency:
                            SPWM_Freq(Freq_Prec+1);
                            break;
                    }
                    break;
                case 'B':
                    switch(Current_Select){
                        case Current:
                            settingCurrent -= 50;
                                break;//?????
                        case Voltage:break;//?????
                        case Freqency:
                            SPWM_Freq(Freq_Prec-1);
                            break;
                    }
                    break;
                case 'C':
                case 'D':
                    break;
                }
            }
            Update_UI();
        }
        if(TA1CTL & TAIFG){
            AVG_RP_COUNT += 1;
        }
        if (TA1CTL & TAIFG && GFX_Changed)
        { //Display Client
            TA1CTL &= ~TAIFG;
            GFX_Changed = 0;
            Nokia5110_Upload();
        }
    };
#endif
}
const char FullFill[7]="\xff\xff\xff\xff\xff\xff";
void Update_UI(){
    GFX_ClearRow_Ranged(1,6,72);
    GFX_ClearRow_Ranged(3,6,78);
    GFX_ClearRow_Ranged(4,52,72);
    GFX_ClearRow(5);
    GFX_WriteAt(RMS_I * 1000,1,6);
    GFX_WriteAt(avg_v,3,6);
    GFX_WriteAt(SPWM_GetFreq(),4,52);
    switch(Current_Select){
        case Current:
            GFX_InvertRow_Ranged(1,6,72);
            break;
        case Voltage:
            GFX_InvertRow_Ranged(3,6,78);
            break;
        case Freqency:
            GFX_InvertRow_Ranged(4,52,72);
            break;
    }
    if(input_status){
        GFX_WriteAt(input_buffer,5,0);  
    }
}

#pragma vector = TIMER0_A1_VECTOR
interrupt void TA0_ISR(){
    TA0CTL &=~ TAIFG;
    if(TB_C<TB_C_MAX){TB_C ++;};
}
#pragma vector = ADC12_VECTOR
interrupt void ADC12_ISR()
{
    ADC12IFG &= ~BIT0;
    if(ADC12MEM0 > ADC12MAX){
        ADC12MAX = ADC12MEM0;
    }
}
#pragma vector = DMA_VECTOR
interrupt void DMA_ISR()
{
    DMA0CTL &= ~DMAIFG;
    ADC12CTL0 &= ~ADC12ENC;
    DMA_ISR_SEND = 1;
}
#endif 

#if 0
#include "vscode.h"
#include "TIDriver/ucs.h"
#include "Modules_Rebuild/Nokia5110.h"
int main(){
    uint8_t contra = 0;

    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_4);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    P2SEL |= BIT2;
    P2DIR |= BIT2;
    P2OUT |= BIT2;
    UCSCTL2 = 0X016F;
    UCSCTL1 = DCORSEL_7;
    UCSCTL3 = SELREF__XT2CLK | FLLREFDIV_2;
    UCSCTL0 = 0X1F << 5;
    UCSCTL7 &=~ DCOFFG;

    P2SEL |= BIT2 | BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT3 + BIT4;
    P5SEL |= BIT4 | BIT5 | BIT2 | BIT3;
    TA0CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA2CCR0 = 1536;
    TA2CCR1 = 500;
    TA2CCR2 = 1000;
    TA2CCTL0 = CM_1 | SCS | SCCI | CAP;
    TA2CCTL1 = CM_2 | SCS | OUTMOD_6;
    TA2CCTL2 = CM_2 | SCS | OUTMOD_6;
    TA2CTL |= MC__UP | TACLR;

    while(1){
    };
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
int main(){
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_4);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    P2SEL |= BIT2;
    P2DIR |= BIT2;
    P2OUT |= BIT2;
    UCSCTL2 = 0X016F;
    UCSCTL1 = DCORSEL_7;
    UCSCTL3 = SELREF__XT2CLK | FLLREFDIV_2;
    UCSCTL0 = 0X1F << 5;
    UCSCTL7 &=~ DCOFFG;
    DPM_Display_Init();
    LCD_Clear(CYAN);
    Display_Asc_String(0,0,5,(uint8_t*)"kksk", RED);
    while(1){
    }
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/Modules/CS5463.H"
// 13250, vrms
int main(){
    volatile _iq24 data;
    volatile float result;
    volatile float result_;
    volatile uint32_t data_;
    volatile uint32_t test2 = CS5463_MaxVoltage;
    volatile uint32_t memory_map[32] = {0};
    Setup_SPI_Master();
    CS5463_Debug_Init();
    data_ = CS5463_ReadVoltage_Gain();
    while (1){
        data = CS5463_Read_RMS_Voltage();
        result = _IQ24toF(_IQ24mpy(_IQ24mpy(data,CS5463_MaxVoltage),CS5463_VoltageRMS__KPL));
        result_ = _IQ24toF(data);
        for(uint16_t i = 0; i < 32; i++){
            memory_map[i] = CS5463_Read_Reg(i, CS5463_Page0);
        }
        __no_operation();
        for(uint16_t i = 0; i < 32; i++){
            CS5463_Write_Reg(i, CS5463_Page0,memory_map[i]);
        }
        __no_operation();
    }
    
}
#endif
#if 1
#include "vscode.h"
#include <msp430.h>
#include "math.h"
#include "Modules/PWM_Burn.h"
#include "Modules_Rebuild/SPWM_int.h"
#endif