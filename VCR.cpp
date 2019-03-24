/*
#include <msp430.h>
#include "oled.h"
#include "UCS.h"
#include "etl/vector.h"
#include "TIDriver/mpy32.h"
#include <float.h>
#include "math.h"
#define double float
#define LOGIC_VOL 3.3
#define LOGIC_VOL_10 33
#define LOGIC_OFF 0
#define OPR 100
#define ORI 0.868476
etl::vector<uint16_t,20> consData;
uint32_t ut_dt = 0;//32BIT, WILL BE FULLED GRETTER THAN 1S
uint16_t Vpeak=0,Vmin=0xffff;//ADC DATA
double Vrms;
uint32_t OV_T = 0;//SYSTEM TIME REFERENCE
uint32_t total_clock = 0;
uint16_t ADC_Data;
uint16_t count = 0;
//ADC 
// INTERRUPT WHEN READY
// MAX SPEED 500US/SAMPLE
//TIMER
// INTERRUPT COMPARE SET TA0,CCR0 AT 32768Hz
// INTERRUPT DIV 4 OV SET TA1, SYSTEM CLOCK REFERENCE
// SET 20FPS, 50MS
// SET 50MS, RECOUNT VPP, Vrms
// SET D[u,t] , dt = nc_us.
// 5%:0.05V
inline uint32_t microMillis(){
    return (OV_T << 16) + TA1R;
}
inline uint32_t resetClock(){
    OV_T = 0;
    TA1CTL |= TACLR;
}
int main(){
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    __disable_interrupt();
    //system clock init
    P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3; 
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0,UCS_XCAP_3);
    UCS_setExternalClockSource(32768,4*Mhz);
    __delay_cycles(1000);
    UCS_initClockSignal(UCS_ACLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
    setupDCO();
    __disable_interrupt();
    //TIMER INIT
    TA0CTL = TASSEL_2 + ID_3 + MC_0 + TACLR;
    TA1CTL = TASSEL_2 + ID_2 + MC_0 + TACLR + TAIE;
    TA0CCTL0 |= CCIE + CM_1;
    TA0CCR0 = 50000u;
    __disable_interrupt();
    //adc12 init
    P6SEL |= 0x01;  
    ADC12CTL0 = ADC12REF2_5V + ADC12ON + ADC12SHT0_8 + ADC12MSC;
    ADC12CTL1 = ADC12SHP+ADC12CONSEQ_2;
    ADC12IE = BIT0;
    __disable_interrupt();
    //mpy32 init
    RES0 = 0u; RES1 = 0u;RES2 = 0u;RES3 = 0u;          //RESET
    MPY32CTL0 =  MPYM_2 + OP1_32 + OP2_32;
    //i2c init
    i2c_master_init();
    //OLED init
    OLED_init();
    OLED_fill(0X00);
    OLED_setXY(0,0);
    oled_print("Vpp:\n");
    oled_print("Vrms:\n");
    oled_print("Vin:\n");
    //SETUP
    TA1CTL |= MC_2;
    TA0CTL |= MC_1;
    ADC12CTL0 |= ADC12ENC;                    // Enable conversions
    ADC12CTL0 |= ADC12SC;                     // Start conversion
	__enable_interrupt();
}

#pragma vector = TIMER1_A1_VECTOR //DIV 4, AT 1MHz, 1us, for system clock reference.
interrupt void VCR_T1A1_ISR(){//EXECUTE 30 CYCLES MAX.
    TA1CTL &=~ TAIFG;
    OV_T ++;
}
#pragma vector = TIMER0_A0_VECTOR  // TACCR0 Less Than 65535us, 1000us per ms, set 50000 for Compare
//high pref.
interrupt void T0A0_ISR(){
    TA0CTL &=~ TAIE;
    ADC12IE = 0X00;
    TA1CTL &=~ TAIE;
    //data = RES3;
    //data = (data << 16) | RES2;
    //data = (data << 16) | RES1;
    //data = (data << 16) | RES0;//GET Integrate[Power[u[t],2],{t,0,T}]
    //d_data += data;
    //d_data /= OPR;
    //d_data /= OPR;
    Vrms = sqrt(Vrms / (double)total_clock);     //CALCULATE Vrms//�� Sum[u^2dt]/T
    if(((double)(Vpeak-Vmin) * (double)LOGIC_VOL / (double)(4096)) - (double)LOGIC_OFF > 0.2){
        Vrms /= ORI;
    }
    //d_data *= 1E-4;                                   //MOVE 1US TO 1S
    //total_clock = 0;                                   
    //RES0 = 0u; RES1 = 0u;RES2 = 0u;RES3 = 0u;          //RESET
    //MAC32H = 0X00;
    //MAC32L = 0X00;
    //OP2L = 0X00;
    //OP2H = 0X00;
    //count = 0;//RESHI=0;RESLO=0;
    //Vrms = d_data;
    //OLED_fill(0X00);
    OLED_setXY(40,0);
    oled_print(((double)(Vpeak-Vmin) * (double)LOGIC_VOL / (double)(4096)) - (double)LOGIC_OFF,4);
    oled_write('V');
    OLED_setXY(40,2);
    oled_print(Vrms,4);
    oled_write('V');
    OLED_setXY(40,4);
    oled_print(((double)ADC_Data * (double)LOGIC_VOL / (double)4096)  - (double)LOGIC_OFF,4);
    oled_write('V');
    Vpeak=0,Vmin=0xffff;//ADC DATA
    Vrms = 0;
    total_clock = 0;
    TA0CCTL0 &=~ CCIFG;
    TA0CTL |= TACLR;
    ADC12IE = 0X01;
    TA1CTL |= TAIE;
}
#pragma vector = ADC12_VECTOR
interrupt void ADC_ISR(){//500us
    uint32_t t0 = microMillis()&0xffff; //WHAT? NEEDN'T DT?
    ADC_Data = ADC12MEM0;
    double data = ((double)ADC_Data * (double)LOGIC_VOL / (double)4096)  - (double)LOGIC_OFF;
    total_clock += 1;
    Vrms += data * data;//u^2dt
    resetClock();
    //t0 *= u2;
    //MAC32L = res & 0x0000ffff;
    //MAC32H = res >> 16;
    //OP2L = res & 0x0000ffff;
    //OP2H = res >> 16;
    //count ++;
    //OP2H = (u2&0xFFFF0000) >> 16; // RSH12, TOTAL 24, USE DOUBLE 64BIT WITH 16 BIT DOT.
    if(Vpeak < ADC_Data){
        Vpeak = ADC_Data;
    }
    if(Vmin > ADC_Data){
        Vmin = ADC_Data;
    }
}*/
