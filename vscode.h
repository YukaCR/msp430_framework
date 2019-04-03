#ifdef __VS_CODE__
#ifndef __VS_CODE_H__
#include <stdint-gcc.h>
#include <msp430.h>
#define interrupt
#define nullptr 0x00L
#endif // !__VS_CODE_H__
#endif // __VS_CODE__
#ifndef __VS_CODE_H__
#define __VS_CODE_H__
#include <msp430.h>
#include "TIDriver/UCS.h"
#include <msp430f5xx_6xxgeneric.h>
#define Mhz 1000000L
#define XT1HFOFFG              (0x0004)       /* XT1 High Frequency Oscillator 1 Fault Flag */
int _main();
void setupDCO(void);
void SetVCoreUp (unsigned int level);
int main(){
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;
    setupDCO();
    P5SEL |= BIT4 + BIT5+BIT2 + BIT3;
    UCS_setExternalClockSource(32768, 4*Mhz);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_3);
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    _main();
}
#define main() inline _main()
void SetVCoreUp (unsigned int level)
{
    // Open PMM registers for write access
    PMMCTL0_H = 0xA5;
    // Set SVS/SVM high side new level
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    // Set SVM low side to new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    // Wait till SVM is settled
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    // Clear already set flags
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    // Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;
    // Wait till new level reached
    if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
    // Set SVS/SVM low side to new level
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
    // Lock PMM registers for write access
    PMMCTL0_H = 0x00;
}
void setupDCO(void)
{

      /* Power settings */
      SetVCoreUp(1u);
      SetVCoreUp(2u);
      SetVCoreUp(3u);
      //SetVCoreUp(4u);
	  //SetVCoreUp(5u);


      UCSCTL3 = SELREF__REFOCLK;    // select REFO as FLL source
      UCSCTL6 = XT1OFF | XT2OFF;    // turn off XT1 and XT2

      /* Initialize DCO to 25.00MHz */
      __bis_SR_register(SCG0);                  // Disable the FLL control loop
      UCSCTL0 = 0x0000u;                        // Set lowest possible DCOx, MODx
      UCSCTL1 = DCORSEL_6;                      // Set RSELx for DCO = 50 MHz
      UCSCTL2 = 762u;                            // Set DCO Multiplier for 33.78MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (1023 + 1) * 32768 = 33.78MHz
      UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32*32*25MHz/32768Hz = 781250 = MCLK cycles for DCO to settle
      __delay_cycles(781250u);


      /* Loop until XT1,XT2 & DCO fault flag is cleared */
      do
      {
    	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
      }
      while (SFRIFG1&OFIFG);                    // Test oscillator fault flag


}
#endif
