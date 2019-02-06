/*#ifndef __ADC_12_H__
#define __ADC_12_H__
#include <msp430.h>
#include "UCS.h"
#include "TIDriver/ucs.h"
#include "TLC5615.h"
#include "oled.h"
#include <stdlib.h>
void ADC12_init(){
	P5SEL |= BIT4 | BIT5; 
	P5SEL |= BIT2 | BIT3; 
    P3SEL |= BIT3 + BIT4;
    P2SEL |= BIT7;
    P3DIR |= BIT3;
    P2DIR |= BIT7;
    P6DIR |= BIT6;
    P6OUT |= BIT6;
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    ADC12CTL0 = ADC12SHT1_2 + ADC12SHT0_2 + ADC12MSC + ADC12ON; 
    ADC12CTL1 = ADC12SHS_0 + ADC12SHP + ADC12DIV_0 + ADC12SSEL_3;
    ADC12CTL2 = ADC12RES_2 ;
    ADC12CTL0 |= ADC12ENC;
}
void setupDCO(void)
{

      // Power settings
      SetVCoreUp(1u);
      SetVCoreUp(2u);
      SetVCoreUp(3u);
      //SetVCoreUp(4u);
      //SetVCoreUp(5u);


      UCSCTL3 = SELREF__REFOCLK;    // select REFO as FLL source
      UCSCTL6 = XT1OFF | XT2OFF;    // turn off XT1 and XT2

      // Initialize DCO to 25.00MHz /
      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL0 = 0x0000u;                        // Set lowest possible DCOx, MODx
      UCSCTL1 = DCORSEL_6;                      // Set RSELx for DCO = 50 MHz
      UCSCTL2 = 244u;                            // Set DCO Multiplier for 33.78MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (1023 + 1) * 32768 = 33.78MHz
      UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32*32*25MHz/32768Hz = 781250 = MCLK cycles for DCO to settle
      __delay_cycles(250000u);


      // Loop until XT1,XT2 & DCO fault flag is cleared 
      do
      {
    	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }
      while (SFRIFG1&OFIFG);                    // Test oscillator fault flag


}
int main(){
    WDTCTL = WDTPW | WDTHOLD;
	P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3; 
	P3SEL |= BIT0 + BIT1;
    P6SEL |= BIT5;
	setupDCO();								//Fuck you TI , up to 34Mhz
    //ADC12_init();
    //ADC12_Single_Setup(ADCMCTLx_P6_5,ADC_P6_5);
    OLED_init();
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2;
    ADC12MCTL0 = ADC12INCH_5;
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;
    OLED_fill(0x00);
    while(1){
        OLED_setXY(0,0);
        ADC12CTL0 |= ADC12SC;
        //TLC5615_setValue((double)256*(double)rand()/(double)RAND_MAX);
        oled_write((double)ADC12MEM0 * (double)3.3 / (double)4096,3);
        __delay_cycles(10000000);
    }
}
#endif
*/