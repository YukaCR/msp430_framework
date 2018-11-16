#ifdef USING_MAIN_CURRENT_SAUCE
#include <msp430.h>
//#include "TLC5615.h"
#include "i2c.h"
//#include "FastPID.h"
void slowDCO(void)
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
/*
int main(){
#define MAX_FLR 0x0C00
#define MIN_FLR	0x0B00
#define MAX_FLN 70
    double current;
    uint16_t ADCData,prevADC_Data = 0;
    uint8_t TLCData = 1;
    uint16_t setpoint = 0; //2.2V / 11 / 1 = 200mA
    WDTCTL = WDTPW | WDTHOLD;
	P5SEL |= BIT4 + BIT5;
	P5SEL |= BIT2 + BIT3; 
	P3SEL |= BIT0 + BIT1;
    P6SEL |= BIT5 + BIT0;
    slowDCO(); 
    i2c_master_init();
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;
    ADC12MCTL0 = ADC12INCH_5;
    ADC12MCTL1 = ADC12INCH_0+ADC12EOS;
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;
	i2c_write_block(0x48,0x40,TLCData,5);
    while(1){
        ADCData = ADC12MEM0;
        setpoint = (double)MIN_FLR+(((double)MAX_FLR - (double)MIN_FLR) * (double)ADC12MEM1) / (double)4096;
        if(((ADCData - prevADC_Data)< MAX_FLN)&&ADCData<setpoint + MAX_FLN/4&& ADCData > setpoint - MAX_FLN/4){
        	prevADC_Data = ADCData;
        	setpoint = (double)MIN_FLR+(((double)MAX_FLR - (double)MIN_FLR) * (double)ADC12MEM1) / (double)4096;
        	ADC12CTL0 |= ADC12SC;
            __delay_cycles(2000);
        }
        ADC12CTL0 |= ADC12SC;
        if(ADCData>setpoint){
            TLCData --;
        }
        else if(ADCData<setpoint){
            TLCData ++;
        }
		i2c_write_block(0x48,0x40,TLCData,5);
        if(TLCData>=0xe0){
            TLCData = 0xe0;
        }
        else if(TLCData<=0x10){
            TLCData = 0x10;
        }
        prevADC_Data = ADCData;
        current = (double)ADCData * (double)3.3 / (double)4096;
        __delay_cycles(2000);
    }
}
/*
int main(){
	uint8_t TLCData = 0;
	WDTCTL = WDTPW | WDTHOLD;
	i2c_master_init();
	 while(1){
		 for(TLCData=0x10;TLCData<=0xe0;TLCData++){
			i2c_write_block(0x48,0x40,TLCData,5);
            __delay_cycles(5000);
		 }
	 }
}
*/
#endif
