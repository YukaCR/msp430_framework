#include "DMA_SPI.h"
#include "SPI.h"
#include "TempGFX.h"

#warning "Nokia5110 uses Timer A1 as refresh Timer. Pay attention if some other modules use the Timer A1"

#define P3_5_INUSE
#define P3_6_INUSE
#define P3_2_INUSE

#define CS_Pin      BIT5
#define DC_Pin      BIT6
#define Reset_Pin   BIT2
#define BIS_CS      P3OUT |= CS_Pin
#define BIC_CS      P3OUT &=~ CS_Pin
#define BIR_CS      BIS_CS;BIC_CS
#define BIS_Reset   P3OUT |= Reset_Pin
#define BIC_Reset   P3OUT &=~ Reset_Pin
#define BIS_DC      P3OUT |= DC_Pin
#define BIC_DC      P3OUT &=~ DC_Pin

#define FPS 60
#define Timer_Peroid 48000000 / FPS
#define splash 1

uint8_t GraphicsMemory[6][84];
volatile uint8_t* DMA_SPI_Buffer = (uint8_t*)GraphicsMemory;

void Nokia5110_Upload(){
    BIC_DC;
    BIC_CS;
    spi_send((uint8_t)0x40);
    spi_send((uint8_t)0x80);
    BIS_CS;

    BIS_DC;
    BIC_CS;
    spi_send((uint8_t*)GraphicsMemory,84 * 6);
    BIS_CS;
}
inline void Setup_Nokia5110_Pinout(){
    P3DIR |= CS_Pin | DC_Pin | Reset_Pin;
    P3REN |= CS_Pin | DC_Pin | Reset_Pin;
}
void Nokia5110_setContrast(uint8_t value) {
  // ***CH***
    if (value > 0x7f) value = 0x7f;

    BIC_DC;
    BIC_CS;
    spi_send((uint8_t)0x21);
    spi_send((uint8_t)(0x80 + value));
    spi_send((uint8_t)0x20);
    BIS_CS;
}
inline void Nokia5110_Data(uint8_t c){
    BIS_DC;
    spi_send(c);
};
inline void Nokia5110_Command(uint8_t c){
    BIC_DC;
    spi_send(c);
};
const uint8_t Nokia5110_SetupCommand[]= {
    0x21, 0xc8, 0x12, 0x20, 0x09, 0x0c, 0x40, 0x80
};
void Setup_Nokia5110_Timer(){
    P2SEL |= BIT0;P2DIR |= BIT0;P2DS |= BIT0;
    TA1CTL = TASSEL__SMCLK + ID_0 + MC__STOP;
    TA1CCR0 = (uint16_t)(Timer_Peroid);
    TA1CCR1 = (uint16_t)(TA1CCR0 >> 1);
    TA1CCTL0 = CM_1 + SCS + SCCI + CAP;
    TA1CCTL1 = CM_0 + SCS + OUTMOD_6;
    TA1CTL |= MC__UP;
}
void Setup_Nokia5110(){
    Setup_SPI();
    Setup_Nokia5110_Pinout();
    GFX_SetupMemory((uint8_t*)GraphicsMemory,84,48);

    BIS_CS;
    BIC_DC;
    _delay_cycles(25 * 1000 * 30);
    BIC_Reset;
    _delay_cycles(25 * 100 * 1000);
    BIS_Reset;
    
    BIC_DC;
    BIC_CS;
    spi_send((uint8_t*)Nokia5110_SetupCommand,sizeof(Nokia5110_SetupCommand));
    BIS_CS;

#if !splash
    GFX_Fill(0x00);
#endif

    GFX_Fill(0xf0);// for debug;
    Nokia5110_setContrast(67);

    Nokia5110_Upload();
    Setup_Nokia5110_Timer();

}   
