#if 1
/*
*	use P1_3 P2_3 as counter port
*	1.698Mhz 1%
*/
#ifndef __FREQ_H__
#define __FREQ_H__
#include <msp430.h> 
#include <stdint.h>
#include <math.h>
#include "UCS.h"
#include "TIDriver/ucs.h"
#include "TIDriver/usci_b_i2c.h"
#ifdef VS_CODE
#define uint16_t unsigned int
#define uint32_t long
#define interrupt 
#endif
#define _RADIUS_F0 4*Mhz
uint32_t Fuckyou_TI = 0;
bool Gate = 1 ,SoftwareGat = 0,startflag = 0;
uint32_t N0Result = 0,NxCount = 0,NxResult = 0;
uint16_t N0loop = 0,N0RWS = 0;
bool low_freq = false,low_start=false;
double Nx_DB;
/**
 * main.c
 */
void burnDCO(void);
void cooldownDCO(void);
void SetVCoreUp (unsigned int level);
inline void _freq_start();
inline void enableN0Counter();
inline void enableGateCounter();
inline void disableGateCounter();
inline void disableN0Counter();
inline void enableNxCounter();
inline void disableNxCounter();
inline void disableProcessInterrupt();
inline void enableProcessInterrupt();
void _freq_main(void);
inline void _freq_start();
void _freq_end();
void(*_freq_callback)(double);
inline void _freq_SetCallback(void(*_callback)(double)){
	_freq_callback = _callback;
}

inline void enableN0Counter(){
	TA0CTL = (low_freq?TASSEL_1:TASSEL_2) + ID_0 + MC_2 + TAIE + TACLR;//SMCLK AT 4MHZ
}
inline void enableGateCounter(){
	//TA1R = 32768;
	//startflag = 1;
	TA1CTL = TASSEL_1 + ID_0 + MC_2 + TACLR + TAIE;
}
inline void disableGateCounter(){
	TA1CTL = 0X00;
}
inline void disableN0Counter(){
	TA0CTL &=~ TAIE;
}
inline void enableNxCounter(){
	//P2OUT|=BIT3;
	P2IE|=BIT3;
}
inline void disableNxCounter(){
	P2IE = 0X00;
}
inline void disableProcessInterrupt(){
	P1IE = 0X00;
}
inline void enableProcessInterrupt(){
	//P1OUT|=BIT3;
	P1IE|=BIT3;
}
void _freq_init(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	__enable_interrupt();

	  P2DIR |= BIT2;                            // SMCLK set out to pins
	  P2SEL |= BIT2;
	//P1OUT = BIT3;
	//P1REN = BIT3;
	//P1IFG = 0X00;
	//P1IE |= BIT3;
	/*clock setting*/
	P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3; 
	P3SEL |= BIT0 + BIT1;
	   burnDCO();								//Fuck you TI , up to 34Mhz
    UCS_setExternalClockSource(32768,4000000L);
    UCS_turnOnXT2(UCS_XT2_DRIVE_4MHZ_8MHZ);
    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0,UCS_XCAP_3);
    UCS_setExternalClockSource(32768,4000000L);
    __delay_cycles(1000);
    UCS_initClockSignal(UCS_ACLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);

	__delay_cycles(1000);
	//_freq_start();
}
//functions
inline void _freq_start(){
    UCS_initClockSignal(UCS_ACLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_MCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);
	Gate = 0;
	startflag = 1;
	N0Result = 0;NxCount = 0;NxResult = 0;
	N0loop = 0;N0RWS = 0;
	enableProcessInterrupt();
	disableNxCounter();
	enableGateCounter();
}
void _freq_end(){
	double f0;
	if(low_freq){
		f0 = 32768;
	}
	else{
		f0 = 4000000;
	}
	N0Result = N0loop;
	N0Result <<= 16;
	N0Result |= TA0R;
	Nx_DB = ((double)NxCount * f0)/ (double)N0Result;
	if(Nx_DB < 200){
		low_freq = true;
		Nx_DB -= 12;
		TA0CTL &=~ TASSEL_3;
		TA0CTL |= TASSEL_1;
	}
	else{
		low_freq = false;
		TA0CTL &=~ TASSEL_3;
		TA0CTL |= TASSEL_2;
		Nx_DB -= 7;
	}
	_freq_callback(Nx_DB);
}
//FUCK YOU TI!

/*TEST PASS 03
#pragma vector = TIMER1_A1_VECTOR
interrupt void T1A1_ISR(){
	//TA0CTL |=  TACLR;
	TA1CTL &=~ TAIFG;
	Gate = 1;
	disableGateCounter();
	disableNxCounter();
	enableProcessInterrupt();
	if(NxCount == 0){
		disableProcessInterrupt();
		_freq_callback(0);
	}
	//Fuckyou_TI ++;//32768hz / 32768 , TEST 5S
	//if(Fuckyou_TI > 5){
	//	TA0CTL &=~ TAIE;
	//}
}
 TEST PASS 02 TA1 AT 4MHZ*
#pragma vector = TIMER0_A1_VECTOR
interrupt void T0A1_ISR(){
	//TA0CTL |=  TACLR;
	TA0CTL &=~ TAIFG;
	N0loop ++;
	//Fuckyou_TI ++;//4Mhz / 65535 //RANDOM TEST
	//if(Fuckyou_TI > 200){
	//	TA0CTL &=~ TAIE;
	//}
}
TEST PASS 01 PORT1 INTERRUPT
#pragma vector = PORT1_VECTOR
interrupt void P1_ISR(){
	P1IFG = 0X00;
	//Fuckyou_TI ++;
	NxCount++;

	if(low_freq){
		if(Gate){
			disableProcessInterrupt();
			N0RWS = TA0R;
			disableN0Counter();
			disableNxCounter();
			_freq_end();
		}
		else if(startflag){
			enableN0Counter();
			startflag = 0;
		}
		else {
			__delay_cycles(100);
			if(!(P1IN&BIT3)){
				NxCount --;
			}
		}
	}
	else{

		if(Gate){//Gate = 1, SWGate = 0 not make sense.
			disableProcessInterrupt();
				N0RWS = TA0R;
				disableN0Counter();
				disableNxCounter();
				_freq_end();
		}
			else if(startflag){
				disableProcessInterrupt();
				enableNxCounter();
				enableN0Counter();
			startflag = 0;
		}
	}
	//if(Fuckyou_TI>10000){ //1KHZ TEST FOR 10S
	//	P1IE &=~ BIT3;
	//}
}
 TEST PASS 04 PORT2 INTERRUPT
#pragma vector = PORT2_VECTOR
interrupt void P2_ISR(){
	P2IFG = 0X00;
	NxCount++;
}

 * FUCK TI.SO SLOW 
 * https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/145592?MSP430F5529-Clock-Configuration-for-25Mhz
 */
void cooldownDCO(){


    /* Power settings */
    //SetVCoreUp(1u);
    //SetVCoreUp(2u);
    //SetVCoreUp(3u);
    //SetVCoreUp(4u);
    //SetVCoreUp(5u);


    UCSCTL3 = SELREF__REFOCLK;    // select REFO as FLL source
    //UCSCTL6 = XT1OFF | XT2OFF;    // turn off XT1 and XT2

    /* Initialize DCO to 25.00MHz */
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000u;                        // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_6;                      // Set RSELx for DCO = 50 MHz
    UCSCTL2 = 762u;                            // Set DCO Multiplier for 33.78MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (762 + 1) * 32768 = 33.78MHz
    //UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
    __bic_SR_register(SCG0);                  // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32*32*25MHz/32768Hz = 781250 = MCLK cycles for DCO to settle
    __delay_cycles(781250u);
}
void burnDCO(void)
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
      UCSCTL2 = 1023u;                            // Set DCO Multiplier for 33.78MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (1023 + 1) * 32768 = 33.78MHz
      UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
      __bic_SR_register(SCG0);                  // Enable the FLL control loop

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32*32*25MHz/32768Hz = 781250 = MCLK cycles for DCO to settle
      __delay_cycles(1062500u);


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
#endif
