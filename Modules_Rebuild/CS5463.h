#ifndef cs5463
#define cs5463

#include "SPI.h"
#include "../IQMath/IQmathLib.h"

/*
    CS5463 Power Sensor.  
*/

// CS5463 Pinout Config, Port 6 as Data Port.
#define CS5463_CS_Pin BIT7  //P3.7
#define CS5463_RST_Pin BIT2 //P8.1

#define BIS_CS5463_CS P3OUT |= CS5463_CS_Pin
#define BIS_CS5463_RST P8OUT |= CS5463_RST_Pin

#define BIC_CS5463_CS P3OUT &= ~CS5463_CS_Pin
#define BIC_CS5463_RST P8OUT &= ~CS5463_RST_Pin

// CS5463 Command Code

#define CS5463_VScale 525        //计算电压比例,220V*250mv/110mv=500V
#define CS5463_IScale _IQ24(250 / 10) //计算电流比例

#define READ_MASK 0xBF   //读寄存器时的屏蔽码，与（写）地址相与
#define CMD_SYNC0 0XFE   //结束串口重新初始化
#define CMD_SYNC1 0XFF   //开始串口重新初始化
#define REG_CONFR 0x40   //配置
#define REG_CYCCONT 0x4A //一个计算周期的A/D转换数
#define REG_STATUSR 0x5E //状态
#define REG_MODER 0x64   //操作模式
#define REG_MASKR 0x74   //中断屏蔽
#define REG_CTRLR 0x78   //控制
#define CMD_STARTC 0XE8  //执行连续计算周期

#define REG_VRMSR 0X18   //VRMS
#define REG_IRMSR 0X16   //IRMS
#define REG_Pactive 0X14 //Pactive

uint8_t CS5463_RX_Buff[4]; //CS5463读写缓冲区
uint8_t CS5463_Status = 0x00;     //芯片状态

inline void CS5463_Command(uint8_t data)
{
    BIC_CS5463_CS;
    spi_send((uint8_t)data);
    BIS_CS5463_CS;
}
inline void CS5463_WriteReg(uint8_t addr, uint8_t *data)
{
    BIC_CS5463_CS;
    spi_send((uint8_t)addr);
    __delay_cycles(25 * 50);
    spi_send((uint8_t *)data, 3);
    BIS_CS5463_CS;
}
inline void CS5463_ReadReg(uint8_t addr, uint8_t *data)
{

    BIC_CS5463_CS;
    spi_send((uint8_t)addr);
    __delay_cycles(25 * 50);
    spi_send((uint8_t *)data, 3, data);
    BIS_CS5463_CS;
}
const uint8_t CS5463_Sync[] = {CMD_SYNC1, CMD_SYNC1, CMD_SYNC0};
const uint8_t CS5463_Reg_Setup_Config[] = {0x00, 0x00, 0x01};
const uint8_t CS5463_Reg_Operate_Config[] = {0x00, 0x00, 0x60};
const uint8_t CS5463_Setup_Cycle_Count[] = {0x00, 0x0f, 0xa0};
const uint8_t CS5463_Setup_Pulse[] = {0x00, 0x34, 0x9c};
const uint8_t CS5463_Setup_Status_Reg[] = {0xff, 0xff, 0xff};
const uint8_t CS5463_Setup_Int_Reg[] = {0x80, 0x00, 0x80};
const uint8_t CS5463_Setup_Control_Reg[] = {0x00, 0x00, 0x00};
void Setup_CS5463()
{
    BIC_CS5463_RST;
    __delay_cycles(25 * 200);
    BIS_CS5463_RST;
    __delay_cycles(25 * 100);
    CS5463_WriteReg(CMD_SYNC1, (uint8_t *)CS5463_Sync);
    CS5463_WriteReg(REG_CONFR, (uint8_t *)CS5463_Reg_Setup_Config);
    CS5463_WriteReg(REG_MODER, (uint8_t *)CS5463_Reg_Operate_Config);
    CS5463_WriteReg(REG_CYCCONT, (uint8_t *)CS5463_Setup_Cycle_Count);
    //  CS5463_WriteReg(REG_PULRATE,(uint8_t*)CS5463_Setup_Pulse);
    CS5463_WriteReg(REG_STATUSR, (uint8_t *)CS5463_Setup_Status_Reg);
    CS5463_WriteReg(REG_MASKR, (uint8_t *)CS5463_Setup_Int_Reg);
    CS5463_WriteReg(REG_CTRLR, (uint8_t *)CS5463_Setup_Control_Reg);
    CS5463_Command(CMD_STARTC);
}
inline void CS5463_Reset_Status_Reg()
{
    return CS5463_WriteReg(REG_CONFR, (uint8_t *)CS5463_Reg_Setup_Config);
}
inline uint8_t CS5463_Get_Status_Reg()
{
    CS5463_ReadReg(0x1E, CS5463_RX_Buff);
    if (CS5463_RX_Buff[0] & 0x80)
    { //检测：电流、电压、功率测量是否完毕
        //检测电流/电压是否超出范围
        //检测电流有效值/电压有效值/电能是否超出范围
        if ((CS5463_RX_Buff[0] & 0x03) || (CS5463_RX_Buff[1] & 0x70))
        {
            CS5463_Reset_Status_Reg(); //复位状态寄存器
        }
        else
        {
            CS5463_Status |= 0x01;
        }
    }
    if (CS5463_RX_Buff[2] & 0x80) //检测：温度测量是否完毕
    {
        CS5463_Status |= 0x02; //B0000_0010;
    }
    return CS5463_Status;
}
void CS5463_Get_Current_RMS(){
    _iq24 result;
    CS5463_ReadReg(REG_IRMSR,CS5463_RX_Buff);
    result = _IQ24mpy(
        (_iq24)((*(uint32_t*)CS5463_RX_Buff)>>8),
        CS5463_IScale);
}
#endif