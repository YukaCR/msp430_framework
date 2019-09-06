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
// TimerA 92MHz to 40kHz test Program
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
    UCSCTL0 = 0xff;
    UCSCTL7 &=~ DCOFFG;
#if 1
    P2SEL |= BIT2 | BIT5 | BIT4;
    P2DIR |= BIT5 | BIT4;
    P2DS |= BIT3 + BIT4;
    P5SEL |= BIT4 | BIT5 | BIT2 | BIT3;
    TA0CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA2CCR0 = 2048;
    TA2CCR1 = 1024;
    TA2CCR2 = 1024;
    TA2CCTL0 = CM_1 | SCS | SCCI | CAP;
    TA2CCTL1 = CM_2 | SCS | OUTMOD_6;
    TA2CCTL2 = CM_2 | SCS | OUTMOD_6;
    TA2CTL |= MC__UP | TACLR;
#endif
    while(1){
    };
}
#endif
// UCS 90MHz test program
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
// CS5463 Test Program
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
    while (1){
        data = CS5463_Read_RMS_Current();
        result = _IQ24toF(_IQ24mpy(_IQ24mpy(data - CS5463_CurrentRMS__BPL,CS5463_MaxCurrent),CS5463_CurrentRMS__KPL));
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
// SPWM TEST Program
#if 0
#include "vscode.h"
#include <msp430.h>
#include "DigitalPowerModules/DPM.h"
#include "DigitalPowerModules/DPM_PWMControl_SPWM.h"
int main(){
    volatile _q12 i = 4096;
    volatile _q12 r_i = 4096;
    SetupDPM();
    DPM_PWMC_Setup_PWMControl();
    DPM_PWMC_Change_DutyCycle(i);
    UCS_initClockSignal(UCS_ACLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_4);
    
    TA0CTL = TASSEL__ACLK | ID_0 | MC__CONTINOUS;
    while(1){
        if(r_i != i){
            __no_operation();
            TA0CTL |= TACLR;
            DPM_PWMC_Change_DutyCycle(i);
            __no_operation();
            r_i = i;
        }
    }
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
int main(){
    volatile uint16_t i = 0;
    Setup_DPM();
    DPM_PWMC_Setup_PWMControl();
    while(1){
        DPM_PWMC_Channel2_DutyCycle_Increase();
        if(DPM_Current_DutyCycle.TA2CCR2 > 2048){
            DPM_Current_DutyCycle.TA2CCR2 = 0;
        }
        __delay_cycles(24 * 1000 * 50);
    }
    return 0;
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
#include "DigitalPowerModules/DPM_Display_TFT.h"
#include "DigitalPowerModules/Modules/fontlib/Chinese.h"
int main(){
    Setup_DPM();       
    Lcd_Init();

    // eido... wtf.
    //LCD_Clear(0x00);//without display string 46ms,with display string 23ms

    /* 
    fontlib_displayChar(Chinese, 0,  0, 0);
    fontlib_displayChar(Chinese, 1, 20, 0);
    fontlib_displayChar(Chinese, 2, 40, 0);
    fontlib_displayChar(Chinese, 3, 60, 0);
    fontlib_displayChar(Chinese, 4, 80, 0);
     */
    TA0CTL = TASSEL__ACLK | ID_0 | MC__CONTINOUS;
    UCS_initClockSignal(UCS_ACLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_4);
    
    while(1){
        TA0CTL |= TACLR;
        //LCD_Clear();
        //fontlib_displayChar(comicSansNumber,0,50,50);//146us, 3418mclk. 22us prepare.
                                   // 131.4us, 2121mclk, second edition
                                   // 129.3us , third edition. ouch.
        fontlib_displayString("0123456789,.>",100,0);// 1.32ms / 10 character. seems great.
        __delay_cycles(65564);
        __no_operation();
    }
    return 1;
}

#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM_Measure_BuiltinADC.h"
#include "DigitalPowerModules/DPM_PWMControl_92MHz_40kHz.h"

uint16_t ADC1_Buffer[4];
uint16_t ADC2_Buffer[4];
uint8_t i = 4;
int main(){
    Setup_DPM();
    DPM_PWMC_Setup_PWMControl();
    DPM_Setup_Power_Measure();// Setup P6.0, P6.1 as measure.
    Setup_Debug_Timer();
    
    while(1){
        DPM_PWMC_Channel1_DutyCycle_Increase();
        if(ADC12IFG){
            ADC12IFG = 0;
            ADC1_Buffer[i--] = ADC12MEM0;
            ADC2_Buffer[i] = ADC12MEM1;
        }
        if(!i){
            register uint16_t buffer1 = 0;
            register uint16_t buffer2 = 0;
            i = 4;
            while(i --){
                buffer1 += ADC1_Buffer[i];
                buffer2 += ADC2_Buffer[i];
            }
            buffer1 >>= 2;
            buffer2 >>= 2;
            if(buffer2 > 1365){// uper than 5V
                DPM_PWMC_Channel1_DutyCycle_Decrease();
            }
            else{
                if(buffer1 > 2457 + 40){    // current
                    DPM_PWMC_Channel1_DutyCycle_Increase();
                }
                else if(buffer1 < 2457 - 40){
                    DPM_PWMC_Channel1_DutyCycle_Decrease();
                }
            }
        } 
    }

}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
#include "DigitalPowerModules/Modules/INA260.h"
int main(){
    volatile uint16_t Power;
    Setup_DPM();
    Setup_INA260();
    UCB0CTL1 = UCSSEL__ACLK | UCTXSTT | UCTR;
    __delay_cycles(10);
    UCB0TXBUF = 0x02;
    while(!(UCB0IFG & UCTXIFG));
    UCB0IFG = 0x00;
    UCB0CTL1 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
    UCB0CTL1 |= UCSWRST;
    while(1){   
        UCB0CTL1 = UCSSEL__ACLK | UCTXSTT;  
        while(!(UCB0IFG & UCRXIFG));
        Power = UCB0RXBUF;
        Power <<= 8;
        UCB0IFG = 0x00;
        UCB0CTL1 = UCSSEL__ACLK | UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);
        Power |= UCB0RXBUF;
        UCB0IFG = 0x00;
        UCB0CTL1 = UCSSEL__ACLK | UCSWRST;
        __no_operation();
        Power = 0;
    }
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/Tools/i2c_scan.h"
int main(){

    while(1){
        //I2C_scan_read();
        I2C_scan_write();
    }
}
#endif
#if 0
#include "vscode.h"
#include "DigitalPowerModules/DPM.h"
#include "DigitalPowerModules/Modules/HW_SPI.h"
int main(){
    uint8_t test[4];
    Setup_DPM();
    Setup_SPI_Master();
    UCA0BRW = 13;
    SPI_SetCPOL_CPHA(false,false);
    P7REN |= BIT0;
    P7DIR |= BIT0;
    P7OUT |= BIT0;
    while (1)
    {
        P7OUT &=~ BIT0;
        test[0] = spi_send(0x88);
        test[1] = spi_send(0xf1);
        test[2] = spi_send(0xf2);
        test[3] = spi_send(0xa0);
        P7OUT |= BIT0;
        __delay_cycles(24* 1000);// 1ms
    };
}
#endif
#if 0
#include "DigitalPowerModules/DPM.h"
#include "DigitalPowerModules/Modules/INA260_SPI.h"
int main(){
uint8_t test[4];
    Setup_DPM();
    INA260_Setup();
    while(1){
        INA260_Ready = false;
        P2IE |= BIT6;
    }
}
#endif
