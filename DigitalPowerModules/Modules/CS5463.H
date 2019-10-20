/*
*   Author: YukaCR
*   aka Modules_Rebuild/CS5463.h
*/
#if 0//#ifndef CS5463
#define CS5463
#include "vscode.h"
#include "HW_SPI.h"
#include "../../IQMath/IQmathLib.h"
#define CS5463_RST_GPIO P6OUT
#define CS5463_RST_Port BIT3
#define CS5463_CS_GPIO  P6OUT
#define CS5463_CS_Port  BIT4

#define CS5463_RST_Set CS5463_RST_GPIO|=CS5463_RST_Port
#define CS5463_RST_Clr CS5463_RST_GPIO&=~CS5463_RST_Port
#define CS5463_CS_Set  CS5463_CS_GPIO|=CS5463_CS_Port
#define CS5463_CS_Clr  CS5463_CS_GPIO&=~CS5463_CS_Port


#define CS5463_MaxCurrent _IQ24(5.0f)
#define CS5463_MaxVoltage _IQ24(125.0f)
#define CS5463_VoltageRMS__KPL  _IQ24( 1.0232220609579101 )
#define CS5463_CurrentRMS__KPL  _IQ24( 1.0 )
#define CS5463_CurrentRMS__BPL  0

inline void CS5463_GPIO_Init(){
    P6SEL &=~ BIT3 | BIT4;
    P6DIR |= BIT3 | BIT4;
    P6REN |= BIT3 | BIT4;
    
}
// Current Transformer : 1000:1
// Resistor: 
/*
    Initiates acquiring measurements and calculating results. The device has two modes of acquisition. 
    Modes of acquisition/measurement 
    0 = Perform a single computation cycle 
    1 = Perform continuous computation cycles
 */
inline uint8_t CS5463_StartConv(const uint8_t ContinuesCompute){
    return 0b11100000 | (ContinuesCompute << 3);
}
#define CS5463_Single_Conv      0
#define CS5463_Continuous_Conv  1 
/*
    The serial port can be initialized by asserting CS or by sending three or more consecutive SYNC1 commands followed by a SYNC0 command. 
    The SYNC0 or SYNC1 can also be sent while sending data out.
    0 = Last byte of a serial port re-initialization sequence. 
    1 = Used during reads and serial port initialization.
 */
inline uint8_t CS5463_SYNC(const uint8_t SYNC){
    return 0xfe | SYNC;
}
/*
    If the device is powered-down, Power-Up/Halt will initiate a power on reset. If the part is already powered-on, all computations will be halted.
 */
inline uint8_t CS5463_Power_up_Halt(){
    return 0b10100000;
}
/*
    To conserve power the CS5463 has two power-down states. In stand-by state all circuitry, except the analog/digital clock generators, is turned off. In the sleep state all circuitry, except the command decoder, is turned off. Bringing the CS5463 out of sleep state requires more time than out of stand-by state, because of the extra time needed to re-start and re-stabilize the analog oscillator.
    0b00 = Software Reset 
    0b01 = Halt and enter stand-by power saving state. This state allows quick power-on 
    0b10 = Halt and enter sleep power saving state. 
    0b11 = Reserved
 */
inline uint8_t CS5463_Power_Down(const uint8_t S){
    return 0b10000000 | (S << 3);
}
#define CS5463_SOFT_RESET   0b00
#define CS5463_Halt_StandBy 0b01
#define CS5463_Halt_Sleep   0b10

/*
    The Read/Write informs the command decoder that a register access is required. During a read operation, the addressed register is loaded into an output buffer and clocked out by SCLK. During a write operation, the data is clocked into an input buffer and transferred to the addressed register upon completion of the 24th SCLK.
    W/nR Write/Read Control
        0 = Read
        1 = Write
    Register:
    Page 0:  0x40 ~ 0x7F
        Configureation
        Current_DC_Offset
        Current_Gain
        Voltage_DC_Offset
        Voltage_Gain
        Cycle_Count :Number of A/D conversions used in one computation cycle (N)
        PluseRateE  :Sets the E1, E2 and E3 energy-to-frequency output pulse rate.
        Instantaneous_Current
        Instantaneous_Voltage
        Instantaneous_Power
        Real_Power
        RMS_Current
        RMS_Voltage
        OWR
        Power_Offset
        Status
        Current_AC_Offset
        Voltage_AC_Offset
        Operation_Mode
        Temperature
        Average_Reactive_Power
        Instantaneous_Reactive_Power
        Peak_Current
        Peak_Voltage
        Reactive_Power_Triangle
        Power_Factor
        Interrupt_Mask
        Apparent_Power
        Control
        Harmonic_Active_Power
        Fundamental_Active_Power
        Fundamental_Reactive_Power
    Page 1:  0x20 ~ 0x3F
        Pluse_Width
        No_Load_Threshold
        Temperature_Sensor_Gain
        Temperature_Sensor_Offser
    Page 2:  0x00 ~ 0x1F
        Voltage_Sag_Sample_Interval
        Voltage_Sag_Level
        Current_Fault_Sample_Interval
        Current_Fault_Level
 */
typedef uint8_t CS5463_Register_t;
inline uint8_t CS5463_Registers(const uint8_t Register, uint8_t Write = 0){
    return (Write << 6) | (Register << 1);
}
#define CS5463_Configureation           0
#define CS5463_Current_DC_Offset        1
#define CS5463_Current_Gain             2
#define CS5463_Voltage_DC_Offset        3
#define CS5463_Voltage_Gain             4
#define CS5463_Cycle_Count              5
#define CS5463_PluseRateE               6
#define CS5463_Instantaneous_Current    7
#define CS5463_Instantaneous_Voltage    8   
#define CS5463_Instantaneous_Power      9
#define CS5463_Real_Power               10

#define CS5463_RMS_Current              11
#define CS5463_RMS_Voltage              12
#define CS5463_OWR                      13
#define CS5463_Power_Offset             14
#define CS5463_Status                   15
#define CS5463_Current_AC_Offset        16
#define CS5463_Voltage_AC_Offset        17
#define CS5463_Operation_Mode           18
#define CS5463_Temperature              19
#define CS5463_Average_Reactive_Power   20
#define CS5463_Instantaneous_Reactive_Power 21
#define CS5463_Peak_Current             22
#define CS5463_Peak_Voltage             23
#define CS5463_Reactive_Power_Triangle  24
#define CS5463_Power_Factor             25
#define CS5463_Interrupt_Mask           26
#define CS5463_Apparent_Power           27
#define CS5463_Control                  28  
#define CS5463_Harmonic_Active_Power    29
#define CS5463_Fundamental_Active_Power 30
#define CS5463_Fundamental_Reactive_Power 31

#define CS5463_Pluse_Width                  0
#define CS5463_No_Load_Threshold            1
#define CS5463_Temperature_Sensor_Gain      2
#define CS5463_Temperature_Sensor_Offser    3

#define CS5463_Voltage_Sag_Sample_Interval   6
#define CS5463_Voltage_Sag_Level             7
#define CS5463_Current_Fault_Sample_Interval 10
#define CS5463_Current_Fault_Level           11

/*
    The CS5463 can perform system calibrations. 
    Proper input signals must be applied to the current and voltage channel before performing a designated calibration.
    Designates calibration to be performed 
    01001 = Current channel DC offset 
    01010 = Current channel DC gain 
    01101 = Current channel AC offset 
    ]01110 = Current channel AC gain 
    10001 = Voltage channel DC offset 
    10010 = Voltage channel DC gain 
    10101 = Voltage channel AC offset 
    10110 = Voltage channel AC gain 
    11001 = Current and Voltage channel DC offset 
    11010 = Current and Voltage channel DC gain 
    11101 = Current and Voltage channel AC offset 
    11110 = Current and Voltage channel AC gain
 */
inline uint8_t CS5463_Calibration(const uint8_t CAL){
    return 0b11000000 | CAL;
}

enum CS5463_Pages{
    CS5463_Page0 = 0,
    CS5463_Page1,
    CS5463_Page2,
    CS5463_Page3
} CS5463_Page;
uint32_t CS5463_Read_Reg(const uint8_t Register,const CS5463_Pages Page){
    register uint32_t data = 0;
    CS5463_CS_Clr;
    if(CS5463_Page != Page){
        spi_send16(0x7e00);
        spi_send16(Page);
    }
    data = (((uint32_t)(spi_send16((uint16_t)CS5463_Registers(Register) << 8 | 0xff) & 0xff) << 16) | spi_send16(0xffff));
    CS5463_CS_Set;
    return data;
}
void CS5463_Write_Reg(const uint8_t Register, const CS5463_Pages Page, uint32_t Data){
    CS5463_CS_Clr;
    if(CS5463_Page != Page){
        spi_send16(0x7e00|Page);
        spi_send16(Page);
    }
    spi_send16((uint16_t)CS5463_Registers(Register,true) << 8 | (Data >> 16)&0xff);
    spi_send16((uint16_t)Data);
    CS5463_CS_Set;
}
#define  CS5463_Command(Command) CS5463_CS_Clr;spi_send(Command);CS5463_CS_Set;

/*
    Config register.
    reset value is 0x000001
    only current pga gain is useful.
    Current_Gain_50:
        0: Gain is 10(default)
        1: Gain is 50
 */
inline void CS5463_Configure_Current_Gain(const uint32_t Current_Gain_50 = false){
    return CS5463_Write_Reg(CS5463_Configureation, CS5463_Page0, 0x000001L | (Current_Gain_50 << 16));
}
/*
    Operation Mode register
    reset value is 0x000000
    only high pass filter is useful
    this function enable / disable current and voltage filter as the same time 
    High pass filter:   
        0: High pass filter disabled(default)
        1: High pass filter enabled
 */
inline void CS5463_Configure_HighPass_Filter(const bool Enabled){
    return CS5463_Write_Reg(CS5463_Operation_Mode, CS5463_Page0 , (Enabled << 5) | (Enabled << 4));
}
// CS5463 Status and mask register.
#define CS5463_Data_Ready       0b1 << 23
#define CS5463_Covertion_Ready  0b1 << 20
#define CS5463_Current_OV       0b1 << 17
#define CS5463_Voltage_OV       0b1 << 16
#define CS5463_Current_RMS_OV   0b1 << 14
#define CS5463_Voltage_RMS_OV   0b1 << 13
#define CS5463_Energy_OV        0b1 << 12
#define CS5463_Current_Fault    0b1 << 11
#define CS5463_Voltage_Sag      0b1 << 10
#define CS5463_Temperature_Ready 0b1 << 7
#define CS5463_Low_Supply_Detect 0b1 << 2
#define CS5463_Epision_Ready    0b1 << 1
#define CS5463_nInvalid_Command 0b1 << 0

inline _iq24 CS5463_Read_RMS_Current(){
    return CS5463_Read_Reg(CS5463_RMS_Current, CS5463_Page0);
}
inline _iq24 CS5463_Read_RMS_Voltage(){
    return CS5463_Read_Reg(CS5463_RMS_Voltage, CS5463_Page0);
}
inline _iq24 CS5463_Read_Active_Power(){
    return CS5463_Read_Reg(CS5463_Real_Power, CS5463_Page0);
}

inline void CS5463_SetVoltage_Gain(const bool Gain_39999){
    return CS5463_Write_Reg(CS5463_Voltage_Gain, CS5463_Page0, Gain_39999?0xffffff:0x400000);
}
inline uint32_t CS5463_ReadVoltage_Gain(){
    return CS5463_Read_Reg(CS5463_Voltage_Gain, CS5463_Page0);
}
inline void Init_CS5463(){
    CS5463_GPIO_Init();
    CS5463_RST_Clr;
    __delay_cycles(96 * 200);
    CS5463_RST_Set;
    __delay_cycles(96 * 200);
    Setup_SPI_Master();
    SPI_SetCPOL_CPHA(true,true);
// Sync.
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(0));
// Igain 50
    CS5463_Configure_Current_Gain(true);
// Vgain 5
    CS5463_SetVoltage_Gain(3.0f);
// High Pass Filter
    CS5463_Configure_HighPass_Filter(false);
// Reset CS5463 Cycle Count
    CS5463_Write_Reg(CS5463_Cycle_Count,CS5463_Page0, 0x000fa0L);
// Reset Status Register.
    CS5463_Write_Reg(CS5463_Status, CS5463_Page0, 0xffffffL);
// Enable Interrupt Signal
    CS5463_Write_Reg(CS5463_Interrupt_Mask, CS5463_Page0,
                     CS5463_Data_Ready);
//  Reset Control Register
    CS5463_Write_Reg(CS5463_Control, CS5463_Page0, 0L);
//  Enable Continus Convertion
    CS5463_Command(CS5463_StartConv(CS5463_Continuous_Conv));
}

void CS5463_Debug_Init(){
    CS5463_GPIO_Init();
    CS5463_RST_Clr;
    __delay_cycles(96 * 200);
    CS5463_RST_Set;
    __delay_cycles(96 * 200);
    Setup_SPI_Master();
    SPI_SetCPOL_CPHA(true,true);
// Sync.
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(1));
    CS5463_Command(CS5463_SYNC(0));
// IGain 10
    CS5463_Configure_Current_Gain(true);
    CS5463_Configure_HighPass_Filter(true);
    CS5463_SetVoltage_Gain(true);
    CS5463_Write_Reg(CS5463_Cycle_Count, CS5463_Page0, 4000);
    CS5463_Write_Reg(CS5463_Status, CS5463_Page0, 0XFFFFFFFFL);
    CS5463_Write_Reg(CS5463_Interrupt_Mask, CS5463_Page0, 0);
    CS5463_Write_Reg(CS5463_Control, CS5463_Page0, 0);
    

    CS5463_Command(CS5463_StartConv(CS5463_Continuous_Conv));
}

#endif
