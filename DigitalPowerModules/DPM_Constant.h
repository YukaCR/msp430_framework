#ifndef DPM_Constant
#define DPM_Constant
#include "../TIDriver/ucs.h"
#define DPM_Output_Mode_Voltage 0
#define DPM_Output_Mode_Current 1
#define DPM_Output_Mode_Power   2
#define DPM_Output_Mode_Notsure 2

// F5529 PinFunction def
#define Perhenial   (0x01 << 0) | 0x01 << 8
#define Output      0x01 << 1
#define Input       0x00 << 1
#define Pullup      0x01 << 2 
#define StatusHigh  (0x01 << 4)
#define DriveStr    0x01 << 5

#define Interrupt   (0x01 << 6) | (0x01 << 7)
#define InterruptHi 0x01 << 6
#define InterruptLo 0x01 << 7
#define InterruptBo (0x01 << 6) | (0x01 << 7)

#define USCIA_SPI   0x01 << 8
#define USCIB_I2C   0x01 << 9
#define USCIB_SPI   0x01 << 10
#define Analog      0x01 << 11
#define Timer       0x01 << 12
#define GPIO        0x01 << 13
#define Clock       0x01 << 14
#define DMA         0x01 << 15

#define PerhenialEnable 0x01
#define PerhenialDisable 0x00

#define Normal OUTMOD_6
#define Reverse OUTMOD_2

#define TA2CLK      50000000 // 50 MHz
#define DCOCLK      92600000 // 92.6 MHz
#define XT1CLK         32768 // 32768 Hz
#define XT2CLK       4000000 // 4  MHz

#define MCLK        23150400 // 23.15 MHz
#define SMCLK       92600000 // 92.6MHz
#define ACLK         4000000 // 4MHz

#endif