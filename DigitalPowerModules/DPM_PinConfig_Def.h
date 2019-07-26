#ifndef Pin_Def
#define Pin_Def
#include "DPM_Constant.h"
// Left 1
// +3.3V
#define P6_5    Analog                      // GPIO
#define P3_4    USCIA_SPI | Output | Pullup // GPIO
#define P3_3    USCIA_SPI | Output | Pullup // GPIO
#define P1_6    GPIO                        // !CS5463 INT. Timer
#define P6_6    Analog                      // GPIO
#define P3_2    GPIO | Output | Pullup      // !TFT in use, USCIA_SPI USCIB_SPI
#define P2_7    USCIA_SPI | Output | Pullup // GPIO, USCIB_SPI
#define P4_2    GPIO | Output | Pullup      // !TFT in use, USCIB_I2C
#define P4_1    GPIO | Output | Pullup      // !TFT in use, USCIB_I2C

// Left 2
// +5V
// GND
#define P6_0    Analog   // GPIO  
#define P6_1    Analog   // GPIO  
#define P6_2    Analog   // GPIO  
#define P6_3    GPIO     // !CS5463 RST Analog
#define P6_4    GPIO     // !CS5463 CS Analog  
#define P7_0    Analog   // GPIO
#define P3_6    GPIO | Output | Pullup      // Timer
#define P3_5    GPIO | Output | Pullup      // !TFT in use, Timer

// Right 1
#define P2_5    Timer | Output | Pullup     // GPIO
#define P2_4    Timer | Output | Pullup     // GPIO
#define P1_5    GPIO  | Pullup | StatusHigh // Timer
#define P1_4    GPIO  | Pullup | StatusHigh // Timer
#define P1_3    GPIO  | Pullup | StatusHigh // Timer
#define P1_2    GPIO  | Pullup | StatusHigh // Timer
#define P4_3    GPIO  | Output              // USCIB_SPI
#define P4_0    GPIO  | Output              // USCIB_SPI
#define P3_7    GPIO  | Output 
#define P8_2    GPIO  | Output

// Right 2
// GND
#define P2_0    GPIO  // Timer
#define P2_2    Timer // GPIO, Clock
#define P7_4    GPIO  // Timer
// RST
#define P3_0    USCIB_I2C   // GPIO, USCIB_SPI
#define P3_1    USCIB_I2C   // GPIO, USCIB_SPI
#define P2_6    GPIO    // Clock, DMA
#define P2_3    GPIO    // Timer
#define P8_1    GPIO

// UCS
#define MCLK        UCS_DCOCLK_SELECT
#define SMCLK       UCS_DCOCLKDIV_SELECT
#define ACLK        UCS_XT2CLK_SELECT
void DPM_UCS_Init();

// Timer
#define TimerA0     PerhenialDisable
#define TimerA1     PerhenialDisable
#define TimerA2     PerhenialEnable
#define TimerB0     PerhenialDisable
void DPM_TimerA0_Init();
void DPM_TimerA1_Init();
void DPM_TimerA2_Init();
void DPM_TimerB0_Init();
// ADC
#define ADC12       PerhenialEnable
void DPM_ADC_Init();

// USCI
#define USCIA0      PerhenialEnable
#define USCIB0      PerhenialEnable
#define USCIA1      PerhenialDisable
#define USCIB1      PerhenialDisable

// DMA
#define DMA0   PerhenialEnable
#define DMA1   PerhenialEnable
#define DMA2   PerhenialEnable

#endif