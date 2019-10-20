/*
*   Author: YukaCR
*   ssd1306 i2c with gfx.(lucky monster?)
*   some time works.(push rst if not work.)
*/  
#ifndef SSD1673_H
#define SSD1673_H
#include "DMA_IIC.h"
#include "TempGFX.h"
#include "msp430.h"
#include "splash.h"
#include "cstring"
#include "string"
#define BLACK 0   ///< Draw 'off' pixels
#define WHITE 1   ///< Draw 'on' pixels
#define INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

// Deprecated size stuff for backwards compatibility with old sketches
#define SSD1306_LCDWIDTH 128 ///< DEPRECATED: width w/SSD1306_128_64 defined
#define SSD1306_LCDHEIGHT 64 ///< DEPRECATED: height w/SSD1306_128_64 defined

#define SSD1306_Continuation 0x80
#define SSD1306_Command  0x40
#define SSD1306_NOP 0xE3


struct SSD1306_StatusMemory_Tamplate{
    uint8_t SetContrast[2] = {SSD1306_SETCONTRAST,0x7f};
    uint8_t DisplayAllON = SSD1306_DISPLAYALLON_RESUME;
    uint8_t InverseDisplay = SSD1306_NORMALDISPLAY;
    uint8_t DisplayON = SSD1306_DISPLAYON;
    uint8_t Scrolling = SSD1306_NOP;
    uint8_t ScrollingData[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t AddressMode[2] = {BIT5,BIT1+BIT0};
    uint8_t ColumnAddress[3] = {BIT5|BIT0,0x00,127};
    uint8_t PageAddress[3] = {BIT5|BIT1, 0x00,7};
    uint8_t DisplayStartLine = BIT6;
    uint8_t SegmentRemap = BIT7+BIT5;
    uint8_t MultiplexRatio[2] = {BIT7 + BIT5 +BIT3,0xff};
    uint8_t ComOutputScanDir = BIT7 + BIT6;
    uint8_t DisplayOffset[2] = {SSD1306_SETDISPLAYOFFSET,0x00};
    uint8_t ComPinHardWare[2] = {SSD1306_SETCOMPINS,BIT4};
    uint8_t DisplayClockFreq[2] = {SSD1306_SETDISPLAYCLOCKDIV,0X00};
    uint8_t PerchargePeroid[2] = {SSD1306_SETPRECHARGE,0x22};
    uint8_t VcomHDeselectLevelp[2] = {SSD1306_SETVCOMDETECT,0x20};
    uint8_t PageStartAddress = 0xb0;
    uint8_t LowColumn = 0x00;
    uint8_t HighColumn = BIT4;
    uint8_t StartGDRAMMapping = 0x40;
};

struct SSD1306_StatusMemory
{    
//setuped
    uint8_t StartSSD1306_Con = 0X00;
    uint8_t MultiplexRatio[2] = {BIT7 + BIT5 +BIT3,0x3f};
    uint8_t DisplayOffset[2] = {SSD1306_SETDISPLAYOFFSET,0x00};
    uint8_t DisplayStartLine = BIT6;
    uint8_t SegmentRemap = BIT7+BIT5;
    uint8_t ComOutputScanDir = BIT7 + BIT6;
    uint8_t ComPinHardWare[2] = {SSD1306_SETCOMPINS,0x02};
    uint8_t SetContrast[2] = {SSD1306_SETCONTRAST,0x7f};
    uint8_t DisplayAllON = SSD1306_DISPLAYALLON_RESUME;
    uint8_t InverseDisplay = SSD1306_NORMALDISPLAY;
    uint8_t DisplayClockFreq[2] = {SSD1306_SETDISPLAYCLOCKDIV,0x80};
    uint8_t ChargePump[2] = {SSD1306_CHARGEPUMP,0x14};
    uint8_t DisplayON = SSD1306_DISPLAYON;
    uint8_t setMemoryAddressMode[2] = {SSD1306_MEMORYMODE,0x00};
//other things
    uint8_t PageStartAddress = 0xb0;
    uint8_t LowColumn = ((0&0xf0)>>4)|0x10;
    uint8_t HighColumn = (0&0x0f)|0x01;
} Screen;
const uint8_t oled_init_data[]={0XAE,0X00,0X10,0X40,0X81,
		0XCF,0XA1,0XC8,0XA6,0XA8,0X3F,
		0XD3,0X00,0XD5,0X80,0XD9,0XF1,
		0XDA,0X12,0XDB,0X40,0X20,0X02,
		0X8D,0X14,0XA4,0XA6,0XAF,0XAF,
        0x20,0x00,0xb0,0x10,0x01};
uint8_t GraphicsMemory[8][128];
void Setup_SSD1306()
{
    USCI_B_I2C_initMasterParam i2c_param;
    P3SEL|=BIT0+BIT1;P3DIR|=BIT0+BIT1;P3REN|=BIT0+BIT1;
    i2c_param.dataRate =USCI_B_I2C_SET_DATA_RATE_400KBPS;
    i2c_param.selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK;
    i2c_param.i2cClk = 4000000;
    USCI_B_I2C_initMaster(UCBxBASE, &i2c_param);
    USCI_B_I2C_setMode(UCBxBASE,USCI_B_I2C_TRANSMIT_MODE);
    USCI_B_I2C_setSlaveAddress(UCBxBASE,0x3c);
    USCI_B_I2C_enable(UCBxBASE);
    UCB0IFG&=~UCTXIFG;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    __delay_cycles(100);
    for(uint8_t i = 0; i < sizeof(oled_init_data);i ++){
        UCB0TXBUF  = *(uint8_t*)((uintptr_t)&oled_init_data + i);
        while(!(UCB0IFG&UCTXIFG));
    }
    UCB0CTL1 |= UCTXSTP;
    while(UCB0CTL1&UCTXSTP);
    UCB0CTL1 |= UCSWRST;
    UCB0IFG&=~UCTXIFG;
    setupDMAIIC((uintptr_t)GraphicsMemory,sizeof(GraphicsMemory));
    GFX_SetupMemory((uint8_t*)GraphicsMemory,SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT);
};
void SSD1306_Display(){
    DMA2CTL &=~ (DMAEN + DMAIE);
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL0 |= UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL0 &=~ UCSWRST;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x00;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x22;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0xFF;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x21;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0;
    while(!UCB0IFG&UCTXIFG);
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL0 |= UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL0 &=~ UCSWRST;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x00;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 127;
    while(!UCB0IFG&UCTXIFG);
    UCB0CTL1 |= UCTXSTP;
    UCB0CTL0 |= UCSWRST;
    UCB0CTL0 &=~ UCSWRST;
    UCB0IFG&=~UCTXIFG;
    UCB0CTL1 |= UCTXSTT;
    while(!UCB0IFG&UCTXIFG);
    UCB0TXBUF = 0x40;
    DMA2CTL |= (DMAEN + DMAIE);
}
void SSD1306_Fill(uint32_t Color){
    memset(GraphicsMemory,Color,sizeof(GraphicsMemory));
}
void SSD1306_Clear(){
    return SSD1306_Fill(0x00);
}
#endif
