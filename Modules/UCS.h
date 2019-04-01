#if 0
#ifndef __MSP430__UCS__
#define __MSP430__UCS__
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "TIDriver/ucs.h"
#define XT1HFOFFG              (0x0004)       /* XT1 High Frequency Oscillator 1 Fault Flag */
#define FLLRef 32768
#define Mhz 1000000L
#define Khz 1000L
#define BIT10 0X0A
#define BIT11 0X0B
#define BIT12 0X0C
#define BIT13 0X0D
#define BIT14 0X0E
#define BIT15 0X0F
#define XT2_ENABLE 1
#define XT2_DISABLE 0
inline void DCO_DIV1();
inline void DCO_DIV2();
inline void DCO_DIV4();
inline void DCO_DIV8();
inline void DCO_DIV16();
inline void DCO_DIV32();
inline void DCO_AutoSelect(uint32_t);
inline void DCO_Freq_AutoCalc(uint32_t);
inline void DCO_DIV_Select(uint8_t);
inline void Clock_Init(bool xt2);
void Set_DCO(unsigned int Delta);
void SetVCoreUp(unsigned int);
void setupDCO(void);//25MHz
enum ClockSause{
	DCO,SMCLK,ACLK
};
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
//GENERAL UNIFIED CLOCK SYSTEM
#ifdef __MSP430_HAS_UCS__
inline void Clock_Init(bool xt2)
{
	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	UCSCTL0 = 0x0000; 
	if (xt2)
	{
		P5SEL |= BIT4 | BIT5; 
	}
	P5SEL |= BIT2 | BIT3; 
	do
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		UCSCTL6 &= ~(XT1OFF + XT2OFF);
		// Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      
		// Clear fault flags
	}while (SFRIFG1&OFIFG); 
	UCS_initClockSignal(UCS_ACLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
	// Test oscillator fault flag
	//setupDCO();
}
/*DCO DIV SET D*/
inline void DCO_DIV2()
{
	UCSCTL2 &= ~(BIT14 + BIT13 + BIT12); 
	UCSCTL2 |= BIT12;
}
inline void DCO_DIV1()
{
	UCSCTL2 &= ~(BIT14 + BIT13 + BIT12);
}
inline void DCO_DIV4()
{
	UCSCTL2 &= ~(BIT14 + BIT13 + BIT12);
	UCSCTL2 |= BIT13;
}
inline void DCO_DIV8()
{
	UCSCTL2 &= ~(BIT14 + BIT13 + BIT12);
	UCSCTL2 |= BIT12+BIT13;
}
inline void DCO_DIV16()
{
	UCSCTL2 &= ~(BIT14 + BIT13 + BIT12);
	UCSCTL2 |= BIT14 ; 
}
inline void DCO_DIV32()
{
	UCSCTL2 |= (BIT14 + BIT13 + BIT12);
}
inline void DCO_DIV_Select(uint8_t div)
{
	switch (div)
	{
	case 1:DCO_DIV1();break;
	case 2:DCO_DIV2();break;
	case 4:DCO_DIV4();break;
	case 8:DCO_DIV8(); break;
	case 16:DCO_DIV16(); break;
	case 32:DCO_DIV32(); break;
	}
}
inline void DCO_Freq_AutoCalc(uint32_t freq)
{
	uint8_t div = 1;
	uint16_t freq_r = freq;
	if (freq > 25 * Mhz)
	{
		return ;
	}
	while ((freq % FLLRef != 0) && div <= 32)
	{
		freq *= 2;
		div *= 2;
	}
	if (freq >= 25 * Mhz && div == 32)
	{
		freq = freq_r;
		div = 1;
	}

	__bis_SR_register(SCG0);  // Disable the FLL control loop
	UCSCTL0 = 0x00;
	UCSCTL2 = 0x00;
	DCO_DIV_Select(div);
	DCO_AutoSelect(freq);
	freq_r = (freq / FLLRef) - 1;
	_nop();
	UCSCTL2 |= freq_r;
	if (!(freq_r % 2))
	{
		UCSCTL2 &= ~BIT1;
	}
	__bic_SR_register(SCG0);  
	freq_r /= 4;
	while (freq_r--);
	return ;
	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
	// Set DCO Multiplier for 8MHz
	// (N + 1) * FLLRef = Fdco
	// (249 + 1) * 32768 = 8MHz
	// Enable the FLL control loop
}
inline void DCO_AutoSelect(uint32_t freq)
{
	/*	DCOSEL_0:	0.20Mhz - 0.70Mhz
	 *	DCOSEL_1:	0.36Mhz - 1.47Mhz
	 *	DCOSEL_2:	0.75Mhz - 3.17Mhz
	 *	DCOSEL_3:	1.51Mhz - 6.07Mhz
	 *	DCOSEL_4:	3.20Mhz - 12.3Mhz
	 *	DCOSEL_5:	6.00Mhz - 23.7Mhz
	 *	DCOSEL_6:	10.7Mhz - 39.0Mhz
	 *	DCOSEL_7:	19.6Mhz - 60.0Mhz
	 */
	if (freq < 700 * Khz)
	{
		UCSCTL1 = DCORSEL_0;
	}
	else if (freq < 1470 * Khz)
	{
		UCSCTL1 = DCORSEL_1;
	}
	else if (freq < 3170 * Khz)
	{
		UCSCTL1 = DCORSEL_2;
	}
	else if (freq < 6070 * Khz)
	{
		UCSCTL1 = DCORSEL_3;
	}
	else if (freq < 12300 * Khz)
	{
		UCSCTL1 = DCORSEL_4;
	}
	else if (freq < 23700 * Khz)
	{
		UCSCTL1 = DCORSEL_5;
	}
	else if (freq < 39 * Mhz)
	{
		UCSCTL1 = DCORSEL_6;
	}
	else
	{
		UCSCTL1 = DCORSEL_7;
	}
}
#endif
//g2 Basic Clock Module Plus
//g2 has LFXT1 XT2 DCO VLO
#ifdef __MSP430_HAS_BC2__

#define DELTA_1MHZ    244                   // 244 x 4096Hz = 999.4Hz
#define DELTA_8MHZ    1953                  // 1953 x 4096Hz = 7.99MHz
#define DELTA_12MHZ   2930                  // 2930 x 4096Hz = 12.00MHz
#define DELTA_16MHZ   3906                  // 3906 x 4096Hz = 15.99MHz
inline void DCO_1Mhz(){
	  if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
	  {
	    while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_1MHZ;                    // Set range
	  DCOCTL = CALDCO_1MHZ;                     // Set DCO step + modulation */

}
inline void DCO_8Mhz(){
	 if (CALBC1_8MHZ==0xFF)					// If calibration constant erased
	  {
	    while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_8MHZ;                    // Set range
	  DCOCTL = CALDCO_8MHZ;                     // Set DCO step + modulation */

}
inline void DCO_12Mhz(){
	if (CALBC1_12MHZ==0xFF)					// If calibration constant erased
	  {
		while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_12MHZ;                   // Set range
	  DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation*/
}
inline void DCO_16Mhz(){
	if (CALBC1_16MHZ==0xFF)					// If calibration constant erased
	  {
	    while(1);                               // do not load, trap CPU!!
	  }
	  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	  BCSCTL1 = CALBC1_16MHZ;                   // Set range
	  DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/
}
/*
void Set_DCO(unsigned int Delta)            // Set DCO to selected frequency
{
	  unsigned int Compare, Oldcapture = 0;
	  BCSCTL1 |= DIVA_3;                        // ACLK = LFXT1CLK/8
	  TACCTL0 = CM_1 + CCIS_1 + CAP;            // CAP, ACLK
	  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

	  while (1)
	  {
	    while (!(CCIFG & TACCTL0));             // Wait until capture occured
	    TACCTL0 &= ~CCIFG;                      // Capture occured, clear flag
	    Compare = TACCR0;                       // Get current captured SMCLK
	    Compare = Compare - Oldcapture;         // SMCLK difference
	    Oldcapture = TACCR0;                    // Save current captured SMCLK

	    if (Delta == Compare)
	      break;                                // If equal, leave "while(1)"
	    else if (Delta < Compare)
	    {
	      DCOCTL--;                             // DCO is too fast, slow it down
	      if (DCOCTL == 0xFF)                   // Did DCO roll under?
	        if (BCSCTL1 & 0x0f)
	          BCSCTL1--;                        // Select lower RSEL
	    }
	    else
	    {
	      DCOCTL++;                             // DCO is too slow, speed it up
	      if (DCOCTL == 0x00)                   // Did DCO roll over?
	        if ((BCSCTL1 & 0x0f) != 0x0f)
	          BCSCTL1++;                        // Sel higher RSEL
	    }
	  }
	  TACCTL0 = 0;                              // Stop TACCR0
	  TACTL = 0;                                // Stop Timer_A
	  BCSCTL1 &= ~DIVA_3;                       // ACLK = LFXT1CLK
}*/
inline void DCO_Freq_AutoCalc(uint32_t freq){
	switch(freq){
	case 1*Mhz:
		DCO_1Mhz();
		break;
	case 8*Mhz:
		DCO_8Mhz();
		break;
	case 12*Mhz:
		DCO_12Mhz();
		break;
	case 16*Mhz:
		DCO_16Mhz();
		break;
	default:
		uint8_t FreqDelta = freq / 4096;
		if(FreqDelta > 32*Mhz){
			return;
		}
		Set_DCO(FreqDelta);
	}
}
inline void Clock_Init(bool xt2){
	if(xt2){
		__delay_cycles(0xffff);//65535cycles wait xt1 start
	}
}
inline void SMCLK_SOURCE_DCO(){
#if defined(__MSP430_HAS_BC2__)
	BCSCTL2 &=~ SELS;
#endif
}
#endif
#endif // !__MSP430__UCS__
#endif
