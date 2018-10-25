//USCI_A_API
#include <msp430.h>
#include "TIDriver/usci_a_spi.h"
#include "TIDriver/ucs.h"
#define Mhz 1000000L
#define Khz 1000L
USCI_A_SPI_initMasterParam usci_spi_param;
void UCAxSPI_init(bool Low_enable,bool msb_first,uint32_t speed){
    UCS_setExternalClockSource(32768,4*Mhz);
    UCS_turnOnXT2(UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    usci_spi_param.selectClockSource = USCI_A_SPI_CLOCKSOURCE_SMCLK;
    usci_spi_param.clockSourceFrequency = UCS_getSMCLK();
    usci_spi_param.clockPhase = Low_enable?USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT:USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    usci_spi_param.clockPolarity = USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    usci_spi_param.msbFirst = msb_first?USCI_A_SPI_MSB_FIRST:USCI_A_SPI_LSB_FIRST;
    usci_spi_param.desiredSpiClock = speed;
    USCI_A_SPI_initMaster(USCI_A0_BASE,&usci_spi_param);
    USCI_A_SPI_enable(USCI_A0_BASE);
    __delay_cycles(1000);
}
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

int main(){
    uint16_t i = 0;
    WDTCTL = WDTPW | WDTHOLD;
    setupDCO();
	P5SEL |= BIT4 | BIT5; 
	P5SEL |= BIT2 | BIT3; 
    P3SEL |= BIT3 + BIT4;
    P2SEL |= BIT7;
    P3DIR |= BIT3;
    P6DIR |= BIT6;
    P6OUT |= BIT6;
    UCAxSPI_init(false,true,500*Khz);
    while(1){
        for(i = 0 ; i < 1024; i ++){
            P6OUT &=~ BIT6;
            USCI_A_SPI_transmitData(USCI_A0_BASE,(((i<<2)&0xff00)>>8)|0x00f0);
            USCI_A_SPI_transmitData(USCI_A0_BASE,(i<<2)&0x00ff);
            P6OUT |= BIT6;
            __delay_cycles(3000);

        }
    }
}
