 		  //头文件

#include "/media/CD_E/HDD-R3/User's Desktop/C51Environment/STC15F104W/STC15F104W.h"
#include "/media/CD_E/HDD-R3/User's Desktop/C51Environment/STC15F104W/vscode.h"
#include <string.h>
#define bool boolean
#define uint  unsigned int
#define uchar unsigned char

typedef bit  bool;
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned int   uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   int   int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned long  uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   long  int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */





/*****************************************************************************/
sbit LCM_cs   = P2^0;  //RS
sbit LCM_std  = P2^1;  //SID
sbit LCM_sclk = P2^2;  //SCLK
sbit LCM_psb  = P2^3;   //H=并口; L=串口;
sbit LCM_rst  = P2^4;   //Reset Signal 低电平有效	sbit LCM_rst  = P2^0; 

sbit KEY1  = P2^5;
sbit KEY2  = P2^6;
sbit KEY3  = P2^7;

//sbit beep     = P2^5;
//sbit p2_4     = P2^6;  //定义背光控制口
 char a,b,c;
 char aa,bb,cc,dd,ee;
 char i,q,T=125;
uchar code tab1[]={
"电压            "
"功率            "
"电流            "
"功率因素        "
};
/****************************************************************************/

uchar r[3]={0x00,0x00,0x00};

/******************************************************************
                           接口定义
******************************************************************/
sbit SCLK=P1^1;	 //ck
sbit MOSI=P1^0;	 //DI
sbit MISO=P1^2;	 //DO	   //这个定义对吗？ 
sbit INT=P3^2;
sbit CS=P1^3;
sbit RST=P1^4;	//RST

#define CS5463_VScale       525             //计算电压比例,220V*250mv/110mv=500V
#define CS5463_IScale       (250/10)        //计算电流比例

static uint8 RX_Buff[4];					//CS5463读写缓冲区
uint8 sta;									//芯片状态

#define READ_MASK		0xBF	//读寄存器时的屏蔽码，与（写）地址相与
#define CMD_SYNC0    	0XFE  	//结束串口重新初始化 
#define CMD_SYNC1    	0XFF  	//开始串口重新初始化
#define REG_CONFR   	0x40   	//配置 
#define REG_CYCCONT 	0x4A   	//一个计算周期的A/D转换数 
#define REG_STATUSR 	0x5E 	//状态 
#define REG_MODER   	0x64  	//操作模式 
#define REG_MASKR   	0x74  	//中断屏蔽 
#define REG_CTRLR   	0x78  	//控制 
#define CMD_STARTC   	0XE8  	//执行连续计算周期

#define REG_VRMSR   	0X18  	//VRMS
#define REG_IRMSR   	0X16  	//IRMS
#define REG_Pactive   	0X14  	//Pactive

		  


/*************************************************************
** 函数名称:uDelay
** 函数功能:延时
** 函数参数:j
** 返回值:无
** 创建时间:2009-4-23
** 第一次修改时间:无
**************************************************************/
static  void uDelay(uint8 j)

{ 
  uint8 i;
  for(;j>0;j--)
	{ for(i=0;i<255;i--)
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
static void CS5463CMD(uint8 cmd)
{
 uint8 i;
 SCLK = 1;
 CS = 0;
 i = 0;
 while(i<8)
 {
  	uDelay(50);
	SCLK = 0;
	if(cmd&0x80)MOSI = 1;
	else		MOSI = 0;
	uDelay(50);
	SCLK = 1;			 		//在时钟上升沿，数据被写入CS5463
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
void CS5463WriteReg(uint8 addr,uint8 *p)
{
 uint8 i,j;
 uint8 dat;
 SCLK = 1;
 CS = 0;
 i = 0;
 while(i<8)
 {
  	uDelay(50);
	SCLK = 0;
	if(addr&0x80)MOSI = 1;
	else		 MOSI = 0;
	uDelay(50);
	SCLK = 1;		 			//在时钟上升沿，数据被写入CS5463
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
		if(dat&0x80)MOSI = 1;
		else		MOSI = 0;
		uDelay(50);
		SCLK = 1;		  		//在时钟上升沿，数据被写入CS5463
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
void CS5463ReadReg(uint8 addr,uint8 *p)
{
 uint8 i,j;
 uint8 dat;
 SCLK = 0;
 CS = 0;
 addr &= READ_MASK;
 i = 0;
 while(i<8)
 {
  	uDelay(50);
	SCLK = 0;
	if(addr&0x80)MOSI = 1;
	else		 MOSI = 0;
	uDelay(50);
	SCLK = 1;
	addr <<= 1;				 	//在时钟上升沿，数据被写入CS5463
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
		if(i==7)MOSI = 0;
		else	MOSI = 1;
		SCLK = 0;
		uDelay(50);
		dat <<= 1;			 			
		if(MISO)dat |= 0x01;
		else	dat &= 0xFE;
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
/*************************************************************************************************
** CS5463 应用函数
*************************************************************************************************/
/*************************************************************
** 函数名称:CS5463Init
** 函数功能:CS5463复位和初始化函数
** 函数参数:无
** 创建时间:2009-9-14
** 第一次修改时间:无
**************************************************************/
bit CS5463_Init(void)	   //bit 可以这样吗？ 
{
 RST = 0;
 uDelay(200);
 RST = 1;
 uDelay(100);
//----------------------
//发送同步序列
 RX_Buff[0] = CMD_SYNC1;
 RX_Buff[1] = CMD_SYNC1;
 RX_Buff[2] = CMD_SYNC0;
 CS5463WriteReg(CMD_SYNC1,RX_Buff);	  //#define CMD_SYNC1    	0XFF  	//开始串口重新初始化		
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
 CS5463WriteReg(REG_CONFR,RX_Buff);	 //	#define REG_CONFR   	0x40   	//配置 
//----------------------
//初始化--操作寄存器
 RX_Buff[0] = 0x00; //B0000_0000;  //这是什么鬼 可以这样吗？ 					 	
 RX_Buff[1] = 0x00;//B0000_0000;	// wdnmd. 打开高通滤波器
 RX_Buff[2] = 0x60;//B0110_0000;
 CS5463WriteReg(REG_MODER,RX_Buff);	  //#define REG_MODER   	0x64  	//操作模式 
//----------------------
//初始化--电流 交流偏置 校准寄存器
// RW24XX(RX_Buff,3,EE_IACBIAS,0xA1);
// CS5463WriteReg(REG_IACOFF,RX_Buff);	
//----------------------
//初始化--电流 增益校准寄存器 
// RW24XX(RX_Buff,3,EE_IACGAIN,0xA1);
// CS5463WriteReg(REG_IGN,RX_Buff);		
//----------------------
//初始化--电压 交流偏置 校准寄存器
// RW24XX(RX_Buff,3,EE_VACBIAS,0xA1);
// CS5463WriteReg(REG_VACOFF,RX_Buff);	
//----------------------
//初始化--电压 增益校准寄存器
// RW24XX(RX_Buff,3,EE_VACGAIN,0xA1);
// CS5463WriteReg(REG_VGN,RX_Buff);		
//----------------------
 RX_Buff[0] = 0x00;
 RX_Buff[1] = 0x0F;
 RX_Buff[2] = 0xA0;						//#define REG_CYCCONT 	0x4A   	//一个计算周期的A/D转换数 
 CS5463WriteReg(REG_CYCCONT,RX_Buff);	//初始化--CYCLE COUNT 寄存器,4000
//----------------------
//初始化--脉冲速率寄存器
// RX_Buff[0] = 0x00;
// RX_Buff[1] = 0x34;
// RX_Buff[2] = 0x9C;
// CS5463WriteReg(REG_PULRATE,RX_Buff);	
//----------------------
 RX_Buff[0] = 0xFF;
 RX_Buff[1] = 0xFF;
 RX_Buff[2] = 0xFF;
 CS5463WriteReg(REG_STATUSR,RX_Buff);	//初始化--状态寄存器  #define REG_STATUSR 	0x5E 	//状态 
//----------------------
 RX_Buff[0] = 0x80;						//开电流、电压、功率测量完毕中断
 RX_Buff[1] = 0x00;
 RX_Buff[2] = 0x80;						//开温度测量完毕中断
 CS5463WriteReg(REG_MASKR,RX_Buff);		//初始化--中断屏蔽寄存器    #define REG_MASKR   	0x74  	//中断屏蔽 
//----------------------
 RX_Buff[0] = 0x00;
 RX_Buff[1] = 0x00;
 RX_Buff[2] = 0x00;
 CS5463WriteReg(REG_CTRLR,RX_Buff);		//初始化--控制寄存器   #define REG_CTRLR   	0x78  	//控制  
//----------------------
 CS5463CMD(CMD_STARTC);				   	//启动连续转换	    #define CMD_STARTC   	0XE8  	//执行连续计算周期
 //CS5463_Status = 0;						//初始化任务进程状态
 //Load_Status = 0;
 //CS5463_CrmsSmallCunt = 0;
 //CS5463_CrmsOverCunt = 0;
 return(1);	  	  //只要做完这些步骤就返回true  1
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
 CS5463WriteReg(0x5E,RX_Buff);		//复位状态寄存器	#define REG_STATUSR 	0x5E 	//状态  
}
/*************************************************************
** 函数名称:CS5463_GetStatusReg
** 函数功能:读取状态寄存器函数
** 函数参数:无
** 创建时间:2009-9-15
** 第一次修改时间:无
**************************************************************/
static uint8 CS5463_GetStatusReg(void)
{ 
 uint8 sta=0;
 CS5463ReadReg(0x1E,RX_Buff);	   //1E 是什么？   状态寄存器
 if(RX_Buff[0]&0x80)		   			//检测：电流、电压、功率测量是否完毕
 {
	//检测电流/电压是否超出范围
	//检测电流有效值/电压有效值/电能是否超出范围
	if((RX_Buff[0]&0x03)||(RX_Buff[1]&0x70))
	{
	 	CS5463_ResetStatusReg();		//复位状态寄存器
	}
	else
	{
		sta |= 0x01;//B0000_0001;	//这什么意思 还可以这样写吗？ PT2017-2-8   分隔符吗？ 
	}
 }

 if(RX_Buff[2]&0x80)			   	//检测：温度测量是否完毕
 {
  	sta |=0x02; //B0000_0010;
 } 
 return(sta);	
}  







void DelayM(unsigned int a)       //延时函数 1MS/次	
{
		unsigned char i;
		while( --a != 0)
       {		
		for(i = 0; i < 125; i++);  //一个 ; 表示空语句,CPU空转从0加到125，CPU大概就耗时1毫秒?
		}   				   
}

void Delay(int num)//延时函数
{
	while(num--);
}

/******************************************************************************/
//写指令或数据  （0，指令） （1，数据）
void LCM_WriteDatOrCom(bit dat_comm,uchar content)
 {
  uchar a,i,j;
  Delay(50);
  a=content;
  LCM_cs=1;
  LCM_sclk=0;
  LCM_std=1;
  for(i=0;i<5;i++)
  {
    LCM_sclk=1;
    LCM_sclk=0;
  }
  LCM_std=0;
  LCM_sclk=1;
  LCM_sclk=0;
  if(dat_comm)
    LCM_std=1;   //data
  else
   LCM_std=0;   //command
  LCM_sclk=1;
  LCM_sclk=0;
  LCM_std=0;
  LCM_sclk=1;
  LCM_sclk=0;
  for(j=0;j<2;j++)
  {
    for(i=0;i<4;i++)
    {
      a=a<<1;
      LCM_std=CY;
      LCM_sclk=1;
      LCM_sclk=0;
    }
    LCM_std=0;
    for(i=0;i<4;i++)
    {
      LCM_sclk=1;
      LCM_sclk=0;
    }
  }
}
/*********************************************************************************/


/*****************************************************************************/
//初始化LCM
void LCM_init(void)	 
{
  LCM_rst=1;
  LCM_psb=0;
  LCM_WriteDatOrCom (0,0x30);  /*30---基本指令动作*/   
  LCM_WriteDatOrCom (0,0x01);  /*清屏，地址指针指向00H*/
  Delay (100);
  LCM_WriteDatOrCom (0,0x06);  /*光标的移动方向*/
  LCM_WriteDatOrCom(0,0x0c);   /*开显示，关游标*/ 
 }

void chn_disp (uchar code *chn)   //显示4行 指针
{
  uchar i,j;
  LCM_WriteDatOrCom  (0,0x30);	 //	0 是指令 这2个命令 是干什么 的？
  LCM_WriteDatOrCom  (0,0x80);	 //
  for (j=0;j<4;j++)
  {
    for (i=0;i<16;i++)
    LCM_WriteDatOrCom  (1,chn[j*16+i]);
  }
}
/*****************************************************************************/
//清屏函数
void LCM_clr(void)
{
  LCM_WriteDatOrCom (0,0x30);
  LCM_WriteDatOrCom (0,0x01);   /*清屏，地址指针指向00H*/
  Delay (180);
}
/*****************************************************************************/
//向LCM发送一个字符串,长度64字符之内。
//应用：LCM_WriteString("您好！"); 
void LCM_WriteString(unsigned char *str)
{
		while(*str != '\0')
       {
			LCM_WriteDatOrCom(1,*str++);
        }
		*str = 0;	  //这里是什么意思 指针归0吗？ 
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
 fp32 G = 0.5,result;
 uint32 temp1;
 uint8 temp,i,j;
 CS5463ReadReg(REG_IRMSR,RX_Buff);   		//读取电流有效值
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
 result = result*CS5463_IScale;//I_Coff;						//计算电流值 暂时不用 
 result *= 1000;								//单位mA(毫安)  12345ma
 temp1 = (uint32)result;

 LCM_WriteDatOrCom  (0,0x94);
	aa=	temp1/10000;
	LCM_WriteDatOrCom(1,aa+0x30);
	bb=	(temp1%10000)/1000;
	LCM_WriteDatOrCom(1,bb+0x30);
	cc=(temp1%1000)/100;
	LCM_WriteDatOrCom(1,cc+0x30);
//	LCM_WriteDatOrCom(1,0x2e);  //小数点  不需要小数点
	dd=	(temp1%100)/10;
	LCM_WriteDatOrCom(1,dd+0x30);
	ee=temp1%10;
	LCM_WriteDatOrCom(1,ee+0x30);
	LCM_WriteString(" mA");

// MeasureData[4] = (uint8)(temp1>>24);
// MeasureData[5] = (uint8)(temp1>>16);
// MeasureData[6] = (uint8)(temp1>>8);
// MeasureData[7] = (uint8)temp1;
// if(0x55==RelayQuery())			  				//检查继电器是否闭合
// {
// 	if(temp1<100)								//检查电流是否小于100mA
// 	{
//		CS5463_CrmsSmallCunt++;
//		if(CS5463_CrmsSmallCunt>10)
//		{
//			CS5463_CrmsSmallCunt = 0;
//			Load_Status = 0x55;					//设置灯状态为：开灯并电流过小
//			if(!CS5463LightFailure_PF)		  	//检查该报警是否已经回应
//			{
//				if(!CS5463LightFailure_nF)		//检查定时重发标志
//				{
//					Comm_ReportLightFailure(MeasureData+4);
//					CS5463LightFailure_nF = 1;
//					CS5463LightFailure_T = 1;
//				}
//			}
//		}
// 	}
// 	else if(temp1>5000)							//过流检测：5000mA
// 	{
//		CS5463_CrmsOverCunt++;
//		if(CS5463_CrmsOverCunt>2)
//		{
//			CS5463_CrmsOverCunt = 0;
//			//RelayCtrl(0);						//断开继电器
//			Comm_ReportOverLoad(MeasureData+4);	//报告过流信息
//		}
// 	}
//	else									  	//如果电流正常
//	{
//		CS5463_CrmsSmallCunt = 0;
// 		CS5463_CrmsOverCunt = 0;
//		Load_Status = 0xAA;						//设置灯状态为：开灯并正常
//		CS5463LightFailure_PF = 0;			   	//复位报警标志
//	}
// }
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
 fp32 G = 1.0,result;
 uint8 temp,i,j;
 uint32 temp1;
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
// result = result*P_Coff;				//计算功率，单位W(瓦特)
// result = Vrms*Irms;					////////直接计算功率
  result = result*13125;
 temp1 = (uint32)result;

  LCM_WriteDatOrCom  (0,0x8C);		//26W  12345W
	aa=	temp1/10000;
	LCM_WriteDatOrCom(1,aa+0x30);
	bb=	(temp1%10000)/1000;
	LCM_WriteDatOrCom(1,bb+0x30);
	cc=(temp1%1000)/100;
	LCM_WriteDatOrCom(1,cc+0x30);
//	LCM_WriteDatOrCom(1,0x2e);  //小数点  不需要小数点
	dd=	(temp1%100)/10;
	LCM_WriteDatOrCom(1,dd+0x30);
	ee=temp1%10;
	LCM_WriteDatOrCom(1,ee+0x30);
	LCM_WriteString(" W");


// MeasureData[8] = (uint8)(temp1>>24);
// MeasureData[9] = (uint8)(temp1>>16);
// MeasureData[10] = (uint8)(temp1>>8);
// MeasureData[11] = (uint8)temp1;
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
 fp32 G = 1.0,result;
 uint8 temp,i,j;
 uint32 temp1;
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
 result *= 10000;
 temp1 = (uint32)result;	   //93.46%
		 LCM_WriteDatOrCom  (0,0x98);
		 LCM_WriteString("功率因素");

        LCM_WriteDatOrCom  (0,0x9C);	// 显示起始位置  第4行 
		aa=	temp1/ 10000;
		LCM_WriteDatOrCom(1,aa+0x30);	 //怎么分离出来显示 PT2017-2-9
		bb=temp1/1000- aa*10;
		LCM_WriteDatOrCom(1,bb+0x30);

		cc=	temp1/100- aa*100-bb*10;
		LCM_WriteDatOrCom(1,cc+0x30);
	    LCM_WriteDatOrCom(1,0x2e);	 //"."
		dd=	(temp1%100)/10;
		LCM_WriteDatOrCom(1,dd+0x30);
		ee=temp1%10;
		LCM_WriteDatOrCom(1,ee+0x30);
		LCM_WriteString("%");
		LCM_WriteString(" ");


// MeasureData[12] = (uint8)(temp1>>24);
// MeasureData[13] = (uint8)(temp1>>16);
// MeasureData[14] = (uint8)(temp1>>8);
// MeasureData[15] = (uint8)temp1;
}

/*************************************************************
** 函数名称:CS5463_GetTemperature
** 函数功能:读取温度函数
** 函数参数:无
** 创建时间:2009-11-03
** 第一次修改时间:无
**************************************************************/
static void CS5463_GetTemperature(void)	  //温度能显示了 PT2017-2-12
{
 fp32 G = 128,result;
 uint8 temp,i,j,pn=0;
 uint32 temp1;
 CS5463ReadReg(0x26,RX_Buff);   		//读取温度	是的在这里就读到了温度 
 //SndCom1Data(RX_Buff,3);
 temp = RX_Buff[0];
 if(temp&0x80)						  	//如果为负数，计算原码
 {
  	pn = 1;								//负数标志
	RX_Buff[0] = ~RX_Buff[0];			//本来为取反+1，这里因为精度的原因，不+1
	RX_Buff[1] = ~RX_Buff[1];
	RX_Buff[2] = ~RX_Buff[2];		 	
 }
 i = 0;
 result = 0;    //这个值是浮点数 先清零 再逐个把0.5的权 数据加进来
 while(i<3)
 {
  	temp = RX_Buff[i];	//虽然这个数组定义了4个字节 实际就用了 Buff[0]  Buff[1]  RX_Buff[2]		   		
	j = 0;
	while(j<8)
	{
	 	if(temp&0x80)
		{
		 	result += G;	//把0.5的权数据加进来		  
		}
		temp <<= 1;
		j++;
		G = G/2;	
	}
	i++;
 }
 if(result<128)			  //是的这个result 是 -127，128   这里已经获取了温度浮点值 最多是一个3位数？ 还有小数点 
 {
 	result *= 100;
	temp1 = (uint32)result;	  //是的 这里就是 例如12523  -----> 125.23  怎么去显示？ 如何分离 从8A开始显示
		 LCM_WriteDatOrCom  (0,0x98);
		 LCM_WriteString("温度    ");
	    
		LCM_WriteDatOrCom  (0,0x9C);	// 显示起始位置  第4行 

		aa=	temp1/ 10000;
		LCM_WriteDatOrCom(1,aa+0x30);	 //怎么分离出来显示 PT2017-2-9
		bb=temp1/1000- aa*10;
		LCM_WriteDatOrCom(1,bb+0x30);

		cc=	temp1/100- aa*100-bb*10;
		LCM_WriteDatOrCom(1,cc+0x30);
	    LCM_WriteDatOrCom(1,0x2e);	 //"."
		dd=	(temp1%100)/10;
		LCM_WriteDatOrCom(1,dd+0x30);
		ee=temp1%10;
		LCM_WriteDatOrCom(1,ee+0x30);
		LCM_WriteString("℃");


//		  PT2017-2-12 显示3个字节的 十六进制数据 
//
//	    LCM_WriteDatOrCom  (0,0x9D);	// 显示起始位置
//			a=RX_Buff[0]/16;
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=RX_Buff[0]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//
//			a=RX_Buff[1]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=RX_Buff[1]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//			
//
//
//			a=RX_Buff[2]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=RX_Buff[2]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}

 }
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
 int temp1;			  //  int 
 uint8 temp,i,j;		  //  byte
 CS5463ReadReg(REG_VRMSR,RX_Buff);   		//读取电压有效值	    这里就读到了吗？  是
 //SndCom1Data(RX_Buff,3);					//#define REG_VRMSR   	0x58	//电压有效值  电压有效值0x58吗？是写
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
 }										//电压在220时取样电压为78mv
result = result*CS5463_VScale;//V_Coff;				//计算电压值220V*250mv/(110mv/1.414)=704.8V	    可以暂时不用  
// if(result<=100)return;					//如果测量读出电压小于100V，确认读数错误
 result *= 100;						//单位为mV（毫伏） 12345mv  5位你怎么显示 
 temp1 = (uint32)result;

	LCM_WriteDatOrCom  (0,0x84);
	aa=	temp1/10000;
	LCM_WriteDatOrCom(1,aa+0x30);
	bb=	(temp1%10000)/1000;
	LCM_WriteDatOrCom(1,bb+0x30);
	cc=(temp1%1000)/100;
	LCM_WriteDatOrCom(1,cc+0x30);
	LCM_WriteDatOrCom(1,0x2e);
	dd=	(temp1%100)/10;
	LCM_WriteDatOrCom(1,dd+0x30);
	ee=temp1%10;
	LCM_WriteDatOrCom(1,ee+0x30);
	LCM_WriteString(" V");



// MeasureData[0] = (uint8)(temp1>>24);
// MeasureData[1] = (uint8)(temp1>>16);
// MeasureData[2] = (uint8)(temp1>>8);
// MeasureData[3] = (uint8)temp1;	
}

void main() 
{
   CS5463_Init();
    LCM_init();       //初始化液晶显示器
	LCM_clr();       //清屏
	chn_disp(tab1); //显示欢迎字
//	DelayM(500);  //显示等留3秒
	// LCM_clr();       //清屏
    
	 
	  while(1)
	  {
	//	if(INT)break;							//检查中断信号

		sta	= CS5463_GetStatusReg();		  	//检测中断产生的原因
		if(0x01==(sta&0x01))		   			//读取电流电压
		{	
	//		CS5463Monitor_Cunt = 0;				//如果有中断，表明芯片正常工作，清除监控定时器
			CS5463_ResetStatusReg();			//清除标志
			CS5463_GetVoltRMS();				//获取电压
			CS5463_GetCurrentRMS();				//获取电流
			CS5463_GetPactiveRMS();				//获取功率
			 if(KEY1==0)
				 {
				CS5463_GetTemperature();
				}
				else
				{
			CS5463_GetPowerFactor();			//获取功率因数
		    	}
			if(0x02==(sta&0x02))				//读取温度
			{	
		//	CS5463_GetVoltRMS();				//获取电压								
		//	CS5463_GetTemperature();		    //温度读取不需要太频繁，所以跟电流电压一起读取

				//CS5463_Init();				//重新初始化芯片
			}	 					
			//SndCom1Data(MeasureData,16);
		}
		else if(0x02==(sta&0x02))
		{
				

				//DelayM(1000);
		}





		//   read_register(0x18,(void *)r); //读取Vrms寄存器值



//			LCM_WriteDatOrCom  (0,0x85);	// 显示起始位置	   PT 2017-2-12 不用显示了
//			a=sta/16;   //这个sta变成0 了怎么办？ 
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=sta%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}


//
//			a=r[1]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[1]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//			
//
//
//			a=r[2]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[2]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//
//
//
//
//			// read_register(0x16,(void *)r); //读取Irms寄存器值，并通过串口发送
//
//			LCM_WriteDatOrCom  (0,0x95);	// 显示起始位置
//			a=r[0]/16;
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[0]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//
//			a=r[1]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[1]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//			
//
//
//			a=r[2]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[2]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}










			 //   read_register(0x14,(void *)r); //读取Irms寄存器值，并通过串口发送

//			LCM_WriteDatOrCom  (0,0x8D);	// 显示起始位置
//			a=r[0]/16;
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[0]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//
//			a=r[1]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[1]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}
//			
//
//
//			a=r[2]/16;
//
//			if(a<10)
//			{
//			LCM_WriteDatOrCom(1,a+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,a+0x37);  //为什么多了7个？ 
//			}
//			b=r[2]%16;
//
//			 if(b<10)
//			{
//			LCM_WriteDatOrCom(1,b+0x30);
//			}
//			else
//			{
//			 LCM_WriteDatOrCom(1,b+0x37);
//			}



		//	  read_register(0x26,(void *)r); //温度 

		//	T=125;	    //对于不确定长度的 怎么办？ 2位小数就很不错了 所以这里就是把小数*100在去显示 int类型变量怎么办？ 
//			LCM_WriteDatOrCom  (0,0x95);	// 显示起始位置
//			aa=T/100;		 //怎么提取出来1呢 ？ 
//			LCM_WriteDatOrCom(1,aa+0x30);	//显示温度十位		 先看懂这里 0-9的就是+0x30 就可以 这里就是写数据  
//			bb=T/10-aa*10;	 //这个2 是怎么提取的呢？ 12-10*1=2 
//			LCM_WriteDatOrCom(1,bb+0x30);	//个位
//			LCM_WriteDatOrCom(1,0x2e);	 //"."
//			cc=T-aa*100-bb*10;	  //再看这个5是如何提取出来的呢？ 125- 100-20 =5  有更好的提取方法吗？ 
//			LCM_WriteDatOrCom(1,cc+0x30); //小位1位
//			LCM_WriteString("℃");	
//			T=T+1;
//			DelayM(50);
		}


}