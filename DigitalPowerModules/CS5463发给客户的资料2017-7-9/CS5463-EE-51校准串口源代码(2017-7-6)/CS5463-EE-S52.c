#include<reg52.h>
#include <intrins.h>
typedef bit  bool;
typedef unsigned long  uint32;
#define uchar unsigned char
#define uint  unsigned int

//#define CS5463_VScale       525             //计算电压比例,220V*250mv/110mv=500V  这里必须2个字节
//#define CS5463_IScale       25              //计算电流比例 (250ma/10)=25    这里1个字节     电流的比例系数25

int VK,IK,PK;

typedef union
{
    float Irms;
    struct
    {
        uchar Irms_dat[4];
    } Irms_suc;
}Irms_Union;

typedef union
{
    float Vrms;
    struct
    {
        uchar Vrms_dat[4];
    } Vrms_suc;
}Vrms_Union;

typedef union
{
    float Pow;
    struct
    {
        uchar Pow_dat[4];
    } Pow_suc;
} Pow_Union;


 typedef union
{
    float PF;
    struct
    {
        uchar PF_dat[4];
    } PF_suc;
} PF_Union;


 typedef union
{
    int EEVB;
    uchar VB_dat[2];
   
} VB_Union;

 typedef union
{
    int EEIB;
    uchar IB_dat[2];
   
} IB_Union;


typedef union
{
    int EEPB;
    uchar PB_dat[2];
   
} PB_Union;

static uchar  RX_Buff[4];                    //CS5463读写缓冲区
uchar sta;                                  //芯片状态
uchar flag;
uint ii=0;
uchar  indata[16];                            //用来存放接收的数据
uchar  outdata[23];							 //发送回应信息
uchar  EE12data[12];
uchar  VAPELdata[16];
uchar inCNT=0;
uchar sum=0;
//要使用必须声明   extern file xdata Para_Save;
Irms_Union  CS5463_Irms;
Vrms_Union  CS5463_Vrms;
Pow_Union   CS5463_Pow;
PF_Union    CS5463_PF;
VB_Union   EVB;
IB_Union   EIB;
PB_Union   EPB;

#define READ_MASK       0xBF    //读寄存器时的屏蔽码，与（写）地址相与
#define CMD_SYNC0       0XFE    //结束串口重新初始化 
#define CMD_SYNC1       0XFF    //开始串口重新初始化
#define REG_CONFR       0x40    //配置 
#define REG_CYCCONT     0x4A    //一个计算周期的A/D转换数 
#define REG_STATUSR     0x5E    //状态 
#define REG_MODER       0x64    //操作模式 
#define REG_MASKR       0x74    //中断屏蔽 
#define REG_CTRLR       0x78    //控制 
#define CMD_STARTC      0XE8    //执行连续计算周期
#define REG_VRMSR       0X18    //VRMS
#define REG_IRMSR       0X16    //IRMS
#define REG_Pactive     0X14    //Pactive
//AT24C02引脚
//sbit scl=P3^4;  //24c02 SCL
//sbit sda=P3^3;  //24c02 SDA

sbit scl=P3^6;  //24c02 SCL
sbit sda=P3^7;  //24c02 SDA
//CS5463模块引脚定义
sbit SCLK=P1^1;         //CK
sbit MOSI=P1^0;         //DI
sbit MISO=P1^2;         //DO
sbit INT=P3^2;          //INT
sbit CS=P1^3;           //CS
sbit RST=P1^4;          //RST
/*************************************************************************************************
** AT24C02相关函数
*************************************************************************************************/
/********************************************************************
* 名称 : flash()
* 功能 : 延时,时间为2个NOP，大概为2US
* 输入 : 无
* 输出 : 无
***********************************************************************/
void flash(void)
{
    _nop_();
    _nop_();
	_nop_();
    _nop_();
	_nop_();
    _nop_();


}
/********************************************************************
* 名称 : x24c02_init()
* 功能 : 24c02初始化子程序
* 输入 : 无
* 输出 : 无
***********************************************************************/
void x24c02_init(void)  //为什么不初始化也可以呢？因为51单片机默认是高电平PT2017-7-2
{
    scl = 1;
    flash();
    sda = 1;
    flash();
}
/********************************************************************
* 名称 : start(void)
* 功能 : 启动I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void start(void)
{
    scl = 1;
    flash();
    sda = 1;
    flash();
    sda = 0;
    flash();
    scl = 0;
    flash();
}
/********************************************************************
* 名称 : stop()
* 功能 : 停止I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void stop()
{
    scl = 0;
    flash();
    sda = 0;
    flash();
    scl = 1;
    flash();
    sda = 1;
    flash();
}
/********************************************************************
* 名称 : writex()
* 功能 : 写一个字节
* 输入 : j（需要写入的值）
* 输出 : 无
***********************************************************************/
void writex(uchar j)
{
    uchar i,temp;
    temp = j;
    for(i=0; i<8; i++)
    {
        scl = 0;
        flash();
        sda = (bit)(temp & 0x80);
        flash();
        scl = 1;
        flash();
        temp = temp << 1;
    }
    scl = 0;
    flash();
}
/********************************************************************
* 名称 : readx()
* 功能 : 读一个字节
* 输入 : 无
* 输出 : 读出的值
***********************************************************************/
uchar readx(void)
{
    uchar i, j, k = 0;
    for(i=0; i<8; i++)
    {
        scl = 0;
        flash();
        if(sda == 1)
        {
            j = 1;
        }
        else
        {
            j = 0;
        }
        k = (k << 1) | j;
        scl = 1;
        flash();
    }
    return(k);
}
/********************************************************************
* 名称 : ack()
* 功能 : I2C总线时钟
* 输入 : 无
* 输出 : 无
***********************************************************************/
void ack(void)
{
    uchar i = 0;
    scl = 1;
    flash();
    while((sda == 1) && (i < 255))
    {
        i++;
    }
    scl = 0;
    flash();
}
/********************************************************************
* 名称 : x24c02_read()
* 功能 : 从24c02中读出值
* 输入 : address(要在这个地址读取值）
* 输出 : 从24c02中读出的值
***********************************************************************/
uchar x24c02_read(uchar address)
{
    uchar i;
    start();
    writex(0xa0);
    ack();
    writex(address);
    ack();
    start();
    writex(0xa1);
    ack();
    i = readx();
    stop();
    return(i);
}
/********************************************************************
* 名称 : x24c02_write()
* 功能 : 想24c02中写入数据
* 输入 : address(地址） ， info（值）
* 输出 : 无
***********************************************************************/
void x24c02_write(uchar address, uchar info)
{
    start();
    writex(0xa0);
    ack();
    writex(address);
    ack();
    writex(info);
    ack();
    stop();
}
//***********************************************************************
// CS5463有关的函数
//***********************************************************************
/******************************************************************
                        串口初始化函数
******************************************************************/
void com_init()
{
    TMOD=0x20;
    TH1=0xfd;
    TL1=0xfd;
    SM0=0;
    SM1=1;
    TR1=1;
    REN=1;
    EA=1;
    ES=1;
}
/*************************************************************
** 函数名称:uDelay
** 函数功能:延时
** 函数参数:j
** 返回值:无
** 创建时间:2009-4-23
** 第一次修改时间:无
**************************************************************/
static  void uDelay(uchar j)
{
    uchar i;
    for(; j>0; j--)
    {
        for(i=0; i<255; i--)
        {
            ;
        }
    }
}
/*************************************************************
** 函数名称:CS5463CMD
** 函数功能:CS5463命令函数
** 函数参数:无
** 创建时间:2009-9-14
** 第一次修改时间:无
**************************************************************/
static void CS5463CMD(uchar cmd)
{
    uchar i;
    SCLK = 1;
    CS = 0;
    i = 0;
    while(i<8)
    {
        uDelay(50);
        SCLK = 0;
        if(cmd&0x80)
        {
            MOSI = 1;
        }
        else
        {
            MOSI = 0;
        }
        uDelay(50);
        SCLK = 1;                   //在时钟上升沿，数据被写入CS5463
        cmd <<= 1;
        i++;
    }
    uDelay(50);
    CS = 1;
}
/*************************************************************
** 函数名称:CS5463WriteReg
** 函数功能:CS5463写寄存器函数
** 函数参数:无
** 创建时间:2009-9-14
** 第一次修改时间:无
**************************************************************/
void CS5463WriteReg(uchar addr,uchar *p)
{
    uchar i,j;
    uchar dat;
    SCLK = 1;
    CS = 0;
    i = 0;
    while(i<8)
    {
        uDelay(50);
        SCLK = 0;
        if(addr&0x80)
        {
            MOSI = 1;
        }
        else
        {
            MOSI = 0;
        }
        uDelay(50);
        SCLK = 1;                   //在时钟上升沿，数据被写入CS5463
        addr <<= 1;
        i++;
    }
    j = 0;
    while(j<3)
    {
        dat = *(p+j);
        i = 0;
        while(i<8)
        {
            uDelay(50);
            SCLK = 0;
            if(dat&0x80)
            {
                MOSI = 1;
            }
            else
            {
                MOSI = 0;
            }
            uDelay(50);
            SCLK = 1;               //在时钟上升沿，数据被写入CS5463
            dat <<= 1;
            i++;
        }
        j++;
    }
    uDelay(50);
    CS = 1;
}
/*************************************************************
** 函数名称:CS5463ReadReg
** 函数功能:CS5463读寄存器函数
** 函数参数:无
** 创建时间:2009-9-14
** 第一次修改时间:无
**************************************************************/
void CS5463ReadReg(uchar addr,uchar *p)
{
    uchar i,j;
    uchar dat;
    SCLK = 0;
    CS = 0;
    addr &= READ_MASK;
    i = 0;
    while(i<8)
    {
        uDelay(50);
        SCLK = 0;
        if(addr&0x80)
        {
            MOSI = 1;
        }
        else
        {
            MOSI = 0;
        }
        uDelay(50);
        SCLK = 1;
        addr <<= 1;                 //在时钟上升沿，数据被写入CS5463
        i++;
    }
    uDelay(50);
    MOSI = 1;
    j = 0;
    while(j<3)
    {
        i = 0;
        dat = 0;
        while(i<8)
        {
            if(i==7)
            {
                MOSI = 0;
            }
            else
            {
                MOSI = 1;
            }
            SCLK = 0;
            uDelay(50);
            dat <<= 1;
            if(MISO)
            {
                dat |= 0x01;
            }
            else
            {
                dat &= 0xFE;
            }
            SCLK = 1;
            uDelay(50);
            i++;
        }
        *(p+j) = dat;
        j++;
    }
    MOSI = 1;
    CS = 1;
}
/*************************************************************
** 函数名称:CS5463Init
** 函数功能:CS5463复位和初始化函数
** 函数参数:无
** 创建时间:2009-9-14
** 第一次修改时间:无
**************************************************************/
bit CS5463_Init(void)      //bit 可以这样吗？
{
    RST = 0;
    uDelay(200);
    RST = 1;
    uDelay(100);
//发送同步序列
    RX_Buff[0] = CMD_SYNC1;
    RX_Buff[1] = CMD_SYNC1;
    RX_Buff[2] = CMD_SYNC0;
    CS5463WriteReg(CMD_SYNC1,RX_Buff);   //#define CMD_SYNC1       0XFF    //开始串口重新初始化
//----------------------
//初始化--配置寄存器
//相位补偿为PC[6:0]=[0000000]；
//电流通道增益为Igain=10；
//EWA=0;
//INT中断为低电平有效IMODE:IINV=[00]
//iCPU=0
//K[3:0]=[0001]
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x01;
    CS5463WriteReg(REG_CONFR,RX_Buff);  // #define REG_CONFR       0x40    //配置
//----------------------
//初始化--操作寄存器
    RX_Buff[0] = 0x00; //B0000_0000;
    RX_Buff[1] = 0x00; //B0000_0000;
    RX_Buff[2] = 0x60; //B0110_0000;
    CS5463WriteReg(REG_MODER,RX_Buff);   //#define REG_MODER       0x64    //操作模式
//初始化--CYCLE COUNT 寄存器,4000
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x0F;
    RX_Buff[2] = 0xA0;                     //#define REG_CYCCONT   0x4A    //一个计算周期的A/D转换数
    CS5463WriteReg(REG_CYCCONT,RX_Buff);   //初始化--CYCLE COUNT 寄存器,4000
//----------------------
    RX_Buff[0] = 0xFF;
    RX_Buff[1] = 0xFF;
    RX_Buff[2] = 0xFF;
    CS5463WriteReg(REG_STATUSR,RX_Buff);   //初始化--状态寄存器  #define REG_STATUSR   0x5E    //状态
//----------------------
    RX_Buff[0] = 0x80;                     //开电流、电压、功率测量完毕中断
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x80;                     //开温度测量完毕中断
    CS5463WriteReg(REG_MASKR,RX_Buff);     //初始化--中断屏蔽寄存器    #define REG_MASKR       0x74    //中断屏蔽
//----------------------
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x00;
    CS5463WriteReg(REG_CTRLR,RX_Buff);     //初始化--控制寄存器   #define REG_CTRLR    0x78    //控制
//----------------------
    CS5463CMD(CMD_STARTC);                 //启动连续转换      #define CMD_STARTC      0XE8    //执行连续计算周期
    return(1);                            //只要做完这些步骤就返回true  1
}
/*************************************************************
** 函数名称:CS5463_ResetStatusReg
** 函数功能:复位状态寄存器函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:无
**************************************************************/
static void CS5463_ResetStatusReg(void)
{
    RX_Buff[0] = 0xFF;
    RX_Buff[1] = 0xFF;
    RX_Buff[2] = 0xFF;
    CS5463WriteReg(0x5E,RX_Buff);      //复位状态寄存器    #define REG_STATUSR     0x5E    //状态
}
/*************************************************************
** 函数名称:CS5463_GetStatusReg
** 函数功能:读取状态寄存器函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:无
**************************************************************/
static uchar CS5463_GetStatusReg(void)
{
    uchar sta=0;
    CS5463ReadReg(0x1E,RX_Buff);      //1E 是什么？   状态寄存器
    if(RX_Buff[0]&0x80)                    //检测：电流、电压、功率测量是否完毕
    {
        //检测电流/电压是否超出范围
        //检测电流有效值/电压有效值/电能是否超出范围
        if((RX_Buff[0]&0x03)||(RX_Buff[1]&0x70))
        {
            CS5463_ResetStatusReg();        //复位状态寄存器
        }
        else
        {
            sta |= 0x01;//B0000_0001;   //这什么意思 还可以这样写吗？ PT2017-2-8   分隔符吗？
        }
    }
    if(RX_Buff[2]&0x80)                //检测：温度测量是否完毕
    {
        sta |=0x02; //B0000_0010;
    }
    return(sta);
}

/*************************************************************
** 函数名称:CS5463_GetVoltRMS
** 函数功能:读取电压有效值函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:2009-9-23，修改电压系数（需验证）
** 第二次修改时间:2010-3-22，设定测量读数小于100V时数据无效
** 第三次修改时间:
**************************************************************/
static void CS5463_GetVoltRMS(void)		 //这个函数啥意思？ 	PT2017-2-12 电压显示OK 
{
 float G = 0.5,result;		//typedef float          fp32;	  就是浮点类型
 uchar temp,i,j;		  //  byte
 CS5463ReadReg(REG_VRMSR,RX_Buff);   		//读取电压有效值	    这里就读到了吗？  是
 i = 0;
 result = 0;
 while(i<3)
 {
  	temp = RX_Buff[i];			   		
	j = 0;
	while(j<8)
	{
	 	if(temp&0x80)
		{
		 	result += G;	
		}
		temp <<= 1;
		j++;
		G = G/2;	
	}
	i++;								
 }										        //电压在220时取样电压为78mv
CS5463_Vrms.Vrms = (result*VK)+EVB.EEVB;        //EVK.VK+EVB.VB; //CS5463_VScale;//V_Coff;		//计算电压值220V*250mv/(110mv/1.414)=704.8V	    可以暂时不用  

 }
/*************************************************************
** 函数名称:CS5463_GetCurrentRMS
** 函数功能:读取电流有效值函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:无
**************************************************************/
static void CS5463_GetCurrentRMS(void)
{
 float G = 0.5,result;
 uchar temp,i,j;
 CS5463ReadReg(REG_IRMSR,RX_Buff);   		//读取电流有效值   0X16    //IRMS
 //SndCom1Data(RX_Buff,3);
 i = 0;
 result = 0;
 while(i<3)
 {
  	temp = RX_Buff[i];			   		
	j = 0;
	while(j<8)
	{
	 	if(temp&0x80)
		{
		 	result += G;	
		}
		temp <<= 1;
		j++;
		G = G/2;	
	}
	i++;
 }

 CS5463_Irms.Irms = (result*IK)+EIB.EEIB;	 //实现y=kx+B
// CS5463_Irms.Irms =result*25;//CS5463_IScale;//I_Coff;	//计算电流值 暂时不用 

}
/*************************************************************
** 函数名称:CS5463_GetPactiveRMS
** 函数功能:读取有功功率函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:无
**************************************************************/
static void CS5463_GetPactiveRMS(void)
{
 float G = 1.0,result;
 uchar temp,i,j;

 CS5463ReadReg(0x14,RX_Buff);   	//读取有功功率REG_Pactive
 //SndCom1Data(RX_Buff,3);
 temp = RX_Buff[0];
 if(temp&0x80)						  	//如果为负数，计算原码
 {
  	RX_Buff[0] = ~RX_Buff[0];			//本来为取反+1，这里因为精度的原因，不+1
	RX_Buff[1] = ~RX_Buff[1];
	RX_Buff[2] = ~RX_Buff[2];		 	
 }
 i = 0;
 result = 0;
 while(i<3)
 {
  	temp = RX_Buff[i];			   		
	j = 0;
	while(j<8) //PT2017-7-7	    原8 
	{
	 	if(temp&0x80)	 //原0x80
		{
		 	result += G;	
		}
		temp <<= 1;
		j++;
		G = G/2;	
	}
	i++;
 }
   CS5463_Pow.Pow = (result*PK)+EPB.EEPB;
 // CS5463_Pow.Pow= result*13125;
}


/*************************************************************
** 函数名称:CS5463_GetPowerFactor
** 函数功能:读取功率因数函数
** 函数参数:无
** 创建时间:2009-11-02
** 第一次修改时间:无
**************************************************************/
static void CS5463_GetPowerFactor(void)
{
 float G = 1.0,result;
 uchar temp,i,j;

 CS5463ReadReg(0x32,RX_Buff);   		//读取功率因数
 //SndCom1Data(RX_Buff,3);
 temp = RX_Buff[0];
 if(temp&0x80)						  	//如果为负数，计算原码
 {
  	RX_Buff[0] = ~RX_Buff[0];			//本来为取反+1，这里因为精度的原因，不+1
	RX_Buff[1] = ~RX_Buff[1];
	RX_Buff[2] = ~RX_Buff[2];		 	
 }
 i = 0;
 result = 0;
 while(i<3)
 {
  	temp = RX_Buff[i];			   		
	j = 0;
	while(j<8)
	{
	 	if(temp&0x80)
		{
		 	result += G;	
		}
		temp <<= 1;
		j++;
		G = G/2;	
	}
	i++;
 }
   CS5463_PF.PF = result;

}

/*************************************************************
** 函数名称:Sendata(Udatlen)	 
** 函数功能: 发送回应信息函数
** 函数参数: 发送个数
** 创建时间:2017-7-3
** 第一次修改时间:无
**************************************************************/

void Sendata(uchar ord,uchar Udatlen,uchar *PP)	 //命令码,发送的个数
{
  
  outdata[0]=0xFB;
  outdata[1]=0xFB;
  outdata[2]=ord;
  outdata[3]=Udatlen;
  for(ii=0;ii<Udatlen;ii++)
  {
  	outdata[4+ii]= PP[ii];
  
  }
  sum=0;
  for(ii=0;ii<Udatlen+4;ii++)
  {
  	sum+= outdata[ii];
  
  }
   outdata[4+Udatlen]=sum;
   outdata[5+Udatlen]=0xFE;
   outdata[6+Udatlen]=0xFE;

   for(ii=0;ii<Udatlen+7;ii++)
   {

          SBUF=outdata[ii];
            while(!TI)
            {
                ;
            }
            TI=0;
   }


}


void RVIPdata(uchar ord,uchar Udatlen,uchar *PPP1,uchar *PPP2,uchar *PPP3,uchar *PPP4)
{

	outdata[0]=0xFB;
  outdata[1]=0xFB;
  outdata[2]=ord;
  outdata[3]=Udatlen;
  for(ii=0;ii<4;ii++)
  {
  	outdata[4+ii]= PPP1[ii];
  
  }
   for(ii=0;ii<4;ii++)
  {
  	outdata[8+ii]= PPP2[ii];
  
  }

  for(ii=0;ii<4;ii++)
  {
  	outdata[12+ii]= PPP3[ii];
  
  }
  for(ii=0;ii<4;ii++)
  {
  	outdata[16+ii]= PPP4[ii];
  
  }

  sum=0;
  for(ii=0;ii<Udatlen+4;ii++)
  {
  	sum+= outdata[ii];
  
  }
   outdata[4+Udatlen]=sum;
   outdata[5+Udatlen]=0xFE;
   outdata[6+Udatlen]=0xFE;

   for(ii=0;ii<Udatlen+7;ii++)
   {

          SBUF=outdata[ii];
            while(!TI)
            {
                ;
            }
            TI=0;
   }



}

void ReadEVIP()	   // CS5463_Vrms.Vrms_suc.Vrms_dat   EVK.VK_suc.VK_dat
{	  
	 
	   
	   VK+= x24c02_read(0)*256+x24c02_read(1);
	    uDelay(100);
	   EVB.VB_dat[0]=x24c02_read(2);
	   EVB.VB_dat[1]=x24c02_read(3);

	   IK+= x24c02_read(4)*256+x24c02_read(5); 
	   uDelay(100);
	   EIB.IB_dat[0]=x24c02_read(6);
	   EIB.IB_dat[1]=x24c02_read(7);


	   PK+= x24c02_read(8)*256+x24c02_read(9);
	    uDelay(100);
	   EPB.PB_dat[0]=x24c02_read(10);
	   EPB.PB_dat[1]=x24c02_read(11);	//是的共12个 



//     VB= x24c02_read(2)*256+	x24c02_read(3);
//     IK= x24c02_read(4)*256+	x24c02_read(5);
//	   IB= x24c02_read(6)*256+	x24c02_read(7);
//	   PK= x24c02_read(8)*256+	x24c02_read(9);
//	   PB= x24c02_read(10)*256+	x24c02_read(11);

}




/*************************************************************************************************
** main主函数
*************************************************************************************************/
void main()
{
    
	com_init();             //串口初始化
    x24c02_init();          //初始化24C02
	CS5463_Init();
	uDelay(200);
	ReadEVIP();
    while(1)
    {
		sta	= CS5463_GetStatusReg();		  	//检测中断产生的原因
		if(0x01==(sta&0x01))		   			//读取电流电压
		{	

			CS5463_ResetStatusReg();			//清除标志
			CS5463_GetVoltRMS();				//获取电压
			CS5463_GetCurrentRMS();				//获取电流
			CS5463_GetPactiveRMS();				//获取功率
			CS5463_GetPowerFactor();			//获取功率因数
		}

		   else if(0x02==(sta&0x02))			//读取温度
			{	
		//	CS5463_GetVoltRMS();				//获取电压								
		//	CS5463_GetTemperature();		    //温度读取不需要太频繁，所以跟电流电压一起读取

			//CS5463_Init();				//重新初始化芯片
			}



        if(flag==1)
        {
            ES=0;
            flag=0;
            switch(VAPELdata[2])
            {
            case 0:    //读12个	电压 电流 功率 KB 的12个字节  为什么没数据返回？ 是不符合FB FB
                for(ii=0; ii<12; ii++)
                {
                    EE12data[ii]=x24c02_read(ii);
                   
                }
				Sendata(VAPELdata[2],12,&EE12data);	 //回应数据的命令码 VAPELdata[2]
                break;
           
            case 1:    //电压校准	 VK VB  写入EE 为什么发这个不对？     FB FB 02 04 02 71 FF CE 3C FE FE
               	 
                  x24c02_write(0,VAPELdata[4]);
				 uDelay(200);
				 x24c02_write(1,VAPELdata[5]);
				  uDelay(200);
				 x24c02_write(2,VAPELdata[6]);
				  uDelay(200);
				 x24c02_write(3,VAPELdata[7]);
				 uDelay(200);
				 VK= VAPELdata[4]*256+VAPELdata[5];
				 EVB.VB_dat[0]=VAPELdata[6];
				 EVB.VB_dat[1]=VAPELdata[7];

                 //VB= VAPELdata[6]*256+VAPELdata[7];  
                break;
	        case 2:    //电流是 4 5 6 7
                 x24c02_write(4,VAPELdata[4]);
				 uDelay(200);
				 x24c02_write(5,VAPELdata[5]);
				  uDelay(200);
				 x24c02_write(6,VAPELdata[6]);
				  uDelay(200);
				 x24c02_write(7,VAPELdata[7]);
				 uDelay(200);
				 IK= VAPELdata[4]*256+VAPELdata[5];
				 EIB.IB_dat[0]=VAPELdata[6];
				 EIB.IB_dat[1]=VAPELdata[7];
                break;

		    case 3:    //功率是 8 9 10 11
                 x24c02_write(8,VAPELdata[4]);
				 uDelay(200);
				 x24c02_write(9,VAPELdata[5]);
				  uDelay(200);
				 x24c02_write(10,VAPELdata[6]);
				  uDelay(200);
				 x24c02_write(11,VAPELdata[7]);
				 uDelay(200);
				 PK= VAPELdata[4]*256+VAPELdata[5];
				 EPB.PB_dat[0]=VAPELdata[6];
				 EPB.PB_dat[1]=VAPELdata[7];
                break;

			  case 4:	//电压显示  要实现y=Kx+b
				
				CS5463_GetVoltRMS();  //读到float类型变量里 K B要放到函数里计算
				
                Sendata(VAPELdata[2],4,&CS5463_Vrms.Vrms_suc.Vrms_dat);	  //这个函数是串口发送数据

                   
                   
                
			  break;

			  case 5:   //电流
				CS5463_GetCurrentRMS();
			    Sendata(VAPELdata[2],4,&CS5463_Irms.Irms_suc.Irms_dat);	
			  break;
			
			  case 6:   //功率
				CS5463_GetPactiveRMS();
			    Sendata(VAPELdata[2],4,&CS5463_Pow.Pow_suc.Pow_dat);	
			  break;  // 

			   case 7:   //是否要加一条可以同时读3个数据 电压 电流功率 功率因数
				CS5463_GetVoltRMS();
				CS5463_GetCurrentRMS();
				 CS5463_GetPactiveRMS();
			    RVIPdata(VAPELdata[2],16,&CS5463_Vrms.Vrms_suc.Vrms_dat,&CS5463_Irms.Irms_suc.Irms_dat,&CS5463_Pow.Pow_suc.Pow_dat,&CS5463_PF.PF_suc.PF_dat);	
			  break;

            default:
                break;




            }
            ES=1;
        }
    }
}
void ser() interrupt 4	//串口中断  把判断协议放在中断里吗？
{
   	if(RI)	    //如果是接收中断
	{
	  RI=0;
	 // P2=SBUF;
	  indata[inCNT]=SBUF;      //需要开辟一个数组来存放接收的数据 indata[] 	
	   inCNT++; 

	   if(inCNT>=6)	//就把长度的字节读进来了
	   {	
	      if((indata[0]==0xFB)&&(indata[1]==0xFB))
		  {

			   if(inCNT>=(indata[3]+7))
			   {
			      sum=0;
			   	  for(ii=0;ii<indata[3]+4;ii++)	  //不包括校验 和包尾 
				  {
				  	sum=sum+ indata[ii];
				  
				  }


				  if((indata[indata[3]+4]==sum)&&(indata[indata[3]+5]==0xFE)&&(indata[indata[3]+6]==0xFE))
				  {

							 for(ii=0;ii<indata[3]+7;ii++)
							 {	
							   VAPELdata[ii]=indata[ii];  //就是全部复制

							  }

				  		  flag=1;	   //这个标志 要是检查包头 长度完整性 和包尾共同决定 
				  		  inCNT=0;
				  }

			   
			   }
		  }
		  else
		  {
		  	  inCNT=0;
		  }

	   
	   } 
	 
    }
}
