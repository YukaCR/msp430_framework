#include<reg52.h>
#include <intrins.h>
typedef bit  bool;
typedef unsigned long  uint32;
#define uchar unsigned char
#define uint  unsigned int

//#define CS5463_VScale       525             //�����ѹ����,220V*250mv/110mv=500V  �������2���ֽ�
//#define CS5463_IScale       25              //����������� (250ma/10)=25    ����1���ֽ�     �����ı���ϵ��25

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

static uchar  RX_Buff[4];                    //CS5463��д������
uchar sta;                                  //оƬ״̬
uchar flag;
uint ii=0;
uchar  indata[16];                            //������Ž��յ�����
uchar  outdata[23];							 //���ͻ�Ӧ��Ϣ
uchar  EE12data[12];
uchar  VAPELdata[16];
uchar inCNT=0;
uchar sum=0;
//Ҫʹ�ñ�������   extern file xdata Para_Save;
Irms_Union  CS5463_Irms;
Vrms_Union  CS5463_Vrms;
Pow_Union   CS5463_Pow;
PF_Union    CS5463_PF;
VB_Union   EVB;
IB_Union   EIB;
PB_Union   EPB;

#define READ_MASK       0xBF    //���Ĵ���ʱ�������룬�루д����ַ����
#define CMD_SYNC0       0XFE    //�����������³�ʼ�� 
#define CMD_SYNC1       0XFF    //��ʼ�������³�ʼ��
#define REG_CONFR       0x40    //���� 
#define REG_CYCCONT     0x4A    //һ���������ڵ�A/Dת���� 
#define REG_STATUSR     0x5E    //״̬ 
#define REG_MODER       0x64    //����ģʽ 
#define REG_MASKR       0x74    //�ж����� 
#define REG_CTRLR       0x78    //���� 
#define CMD_STARTC      0XE8    //ִ��������������
#define REG_VRMSR       0X18    //VRMS
#define REG_IRMSR       0X16    //IRMS
#define REG_Pactive     0X14    //Pactive
//AT24C02����
//sbit scl=P3^4;  //24c02 SCL
//sbit sda=P3^3;  //24c02 SDA

sbit scl=P3^6;  //24c02 SCL
sbit sda=P3^7;  //24c02 SDA
//CS5463ģ�����Ŷ���
sbit SCLK=P1^1;         //CK
sbit MOSI=P1^0;         //DI
sbit MISO=P1^2;         //DO
sbit INT=P3^2;          //INT
sbit CS=P1^3;           //CS
sbit RST=P1^4;          //RST
/*************************************************************************************************
** AT24C02��غ���
*************************************************************************************************/
/********************************************************************
* ���� : flash()
* ���� : ��ʱ,ʱ��Ϊ2��NOP�����Ϊ2US
* ���� : ��
* ��� : ��
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
* ���� : x24c02_init()
* ���� : 24c02��ʼ���ӳ���
* ���� : ��
* ��� : ��
***********************************************************************/
void x24c02_init(void)  //Ϊʲô����ʼ��Ҳ�����أ���Ϊ51��Ƭ��Ĭ���Ǹߵ�ƽPT2017-7-2
{
    scl = 1;
    flash();
    sda = 1;
    flash();
}
/********************************************************************
* ���� : start(void)
* ���� : ����I2C����
* ���� : ��
* ��� : ��
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
* ���� : stop()
* ���� : ֹͣI2C����
* ���� : ��
* ��� : ��
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
* ���� : writex()
* ���� : дһ���ֽ�
* ���� : j����Ҫд���ֵ��
* ��� : ��
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
* ���� : readx()
* ���� : ��һ���ֽ�
* ���� : ��
* ��� : ������ֵ
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
* ���� : ack()
* ���� : I2C����ʱ��
* ���� : ��
* ��� : ��
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
* ���� : x24c02_read()
* ���� : ��24c02�ж���ֵ
* ���� : address(Ҫ�������ַ��ȡֵ��
* ��� : ��24c02�ж�����ֵ
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
* ���� : x24c02_write()
* ���� : ��24c02��д������
* ���� : address(��ַ�� �� info��ֵ��
* ��� : ��
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
// CS5463�йصĺ���
//***********************************************************************
/******************************************************************
                        ���ڳ�ʼ������
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
** ��������:uDelay
** ��������:��ʱ
** ��������:j
** ����ֵ:��
** ����ʱ��:2009-4-23
** ��һ���޸�ʱ��:��
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
** ��������:CS5463CMD
** ��������:CS5463�����
** ��������:��
** ����ʱ��:2009-9-14
** ��һ���޸�ʱ��:��
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
        SCLK = 1;                   //��ʱ�������أ����ݱ�д��CS5463
        cmd <<= 1;
        i++;
    }
    uDelay(50);
    CS = 1;
}
/*************************************************************
** ��������:CS5463WriteReg
** ��������:CS5463д�Ĵ�������
** ��������:��
** ����ʱ��:2009-9-14
** ��һ���޸�ʱ��:��
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
        SCLK = 1;                   //��ʱ�������أ����ݱ�д��CS5463
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
            SCLK = 1;               //��ʱ�������أ����ݱ�д��CS5463
            dat <<= 1;
            i++;
        }
        j++;
    }
    uDelay(50);
    CS = 1;
}
/*************************************************************
** ��������:CS5463ReadReg
** ��������:CS5463���Ĵ�������
** ��������:��
** ����ʱ��:2009-9-14
** ��һ���޸�ʱ��:��
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
        addr <<= 1;                 //��ʱ�������أ����ݱ�д��CS5463
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
** ��������:CS5463Init
** ��������:CS5463��λ�ͳ�ʼ������
** ��������:��
** ����ʱ��:2009-9-14
** ��һ���޸�ʱ��:��
**************************************************************/
bit CS5463_Init(void)      //bit ����������
{
    RST = 0;
    uDelay(200);
    RST = 1;
    uDelay(100);
//����ͬ������
    RX_Buff[0] = CMD_SYNC1;
    RX_Buff[1] = CMD_SYNC1;
    RX_Buff[2] = CMD_SYNC0;
    CS5463WriteReg(CMD_SYNC1,RX_Buff);   //#define CMD_SYNC1       0XFF    //��ʼ�������³�ʼ��
//----------------------
//��ʼ��--���üĴ���
//��λ����ΪPC[6:0]=[0000000]��
//����ͨ������ΪIgain=10��
//EWA=0;
//INT�ж�Ϊ�͵�ƽ��ЧIMODE:IINV=[00]
//iCPU=0
//K[3:0]=[0001]
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x01;
    CS5463WriteReg(REG_CONFR,RX_Buff);  // #define REG_CONFR       0x40    //����
//----------------------
//��ʼ��--�����Ĵ���
    RX_Buff[0] = 0x00; //B0000_0000;
    RX_Buff[1] = 0x00; //B0000_0000;
    RX_Buff[2] = 0x60; //B0110_0000;
    CS5463WriteReg(REG_MODER,RX_Buff);   //#define REG_MODER       0x64    //����ģʽ
//��ʼ��--CYCLE COUNT �Ĵ���,4000
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x0F;
    RX_Buff[2] = 0xA0;                     //#define REG_CYCCONT   0x4A    //һ���������ڵ�A/Dת����
    CS5463WriteReg(REG_CYCCONT,RX_Buff);   //��ʼ��--CYCLE COUNT �Ĵ���,4000
//----------------------
    RX_Buff[0] = 0xFF;
    RX_Buff[1] = 0xFF;
    RX_Buff[2] = 0xFF;
    CS5463WriteReg(REG_STATUSR,RX_Buff);   //��ʼ��--״̬�Ĵ���  #define REG_STATUSR   0x5E    //״̬
//----------------------
    RX_Buff[0] = 0x80;                     //����������ѹ�����ʲ�������ж�
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x80;                     //���¶Ȳ�������ж�
    CS5463WriteReg(REG_MASKR,RX_Buff);     //��ʼ��--�ж����μĴ���    #define REG_MASKR       0x74    //�ж�����
//----------------------
    RX_Buff[0] = 0x00;
    RX_Buff[1] = 0x00;
    RX_Buff[2] = 0x00;
    CS5463WriteReg(REG_CTRLR,RX_Buff);     //��ʼ��--���ƼĴ���   #define REG_CTRLR    0x78    //����
//----------------------
    CS5463CMD(CMD_STARTC);                 //��������ת��      #define CMD_STARTC      0XE8    //ִ��������������
    return(1);                            //ֻҪ������Щ����ͷ���true  1
}
/*************************************************************
** ��������:CS5463_ResetStatusReg
** ��������:��λ״̬�Ĵ�������
** ��������:��
** ����ʱ��:2009-9-15
** ��һ���޸�ʱ��:��
**************************************************************/
static void CS5463_ResetStatusReg(void)
{
    RX_Buff[0] = 0xFF;
    RX_Buff[1] = 0xFF;
    RX_Buff[2] = 0xFF;
    CS5463WriteReg(0x5E,RX_Buff);      //��λ״̬�Ĵ���    #define REG_STATUSR     0x5E    //״̬
}
/*************************************************************
** ��������:CS5463_GetStatusReg
** ��������:��ȡ״̬�Ĵ�������
** ��������:��
** ����ʱ��:2009-9-15
** ��һ���޸�ʱ��:��
**************************************************************/
static uchar CS5463_GetStatusReg(void)
{
    uchar sta=0;
    CS5463ReadReg(0x1E,RX_Buff);      //1E ��ʲô��   ״̬�Ĵ���
    if(RX_Buff[0]&0x80)                    //��⣺��������ѹ�����ʲ����Ƿ����
    {
        //������/��ѹ�Ƿ񳬳���Χ
        //��������Чֵ/��ѹ��Чֵ/�����Ƿ񳬳���Χ
        if((RX_Buff[0]&0x03)||(RX_Buff[1]&0x70))
        {
            CS5463_ResetStatusReg();        //��λ״̬�Ĵ���
        }
        else
        {
            sta |= 0x01;//B0000_0001;   //��ʲô��˼ ����������д�� PT2017-2-8   �ָ�����
        }
    }
    if(RX_Buff[2]&0x80)                //��⣺�¶Ȳ����Ƿ����
    {
        sta |=0x02; //B0000_0010;
    }
    return(sta);
}

/*************************************************************
** ��������:CS5463_GetVoltRMS
** ��������:��ȡ��ѹ��Чֵ����
** ��������:��
** ����ʱ��:2009-9-15
** ��һ���޸�ʱ��:2009-9-23���޸ĵ�ѹϵ��������֤��
** �ڶ����޸�ʱ��:2010-3-22���趨��������С��100Vʱ������Ч
** �������޸�ʱ��:
**************************************************************/
static void CS5463_GetVoltRMS(void)		 //�������ɶ��˼�� 	PT2017-2-12 ��ѹ��ʾOK 
{
 float G = 0.5,result;		//typedef float          fp32;	  ���Ǹ�������
 uchar temp,i,j;		  //  byte
 CS5463ReadReg(REG_VRMSR,RX_Buff);   		//��ȡ��ѹ��Чֵ	    ����Ͷ�������  ��
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
 }										        //��ѹ��220ʱȡ����ѹΪ78mv
CS5463_Vrms.Vrms = (result*VK)+EVB.EEVB;        //EVK.VK+EVB.VB; //CS5463_VScale;//V_Coff;		//�����ѹֵ220V*250mv/(110mv/1.414)=704.8V	    ������ʱ����  

 }
/*************************************************************
** ��������:CS5463_GetCurrentRMS
** ��������:��ȡ������Чֵ����
** ��������:��
** ����ʱ��:2009-9-15
** ��һ���޸�ʱ��:��
**************************************************************/
static void CS5463_GetCurrentRMS(void)
{
 float G = 0.5,result;
 uchar temp,i,j;
 CS5463ReadReg(REG_IRMSR,RX_Buff);   		//��ȡ������Чֵ   0X16    //IRMS
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

 CS5463_Irms.Irms = (result*IK)+EIB.EEIB;	 //ʵ��y=kx+B
// CS5463_Irms.Irms =result*25;//CS5463_IScale;//I_Coff;	//�������ֵ ��ʱ���� 

}
/*************************************************************
** ��������:CS5463_GetPactiveRMS
** ��������:��ȡ�й����ʺ���
** ��������:��
** ����ʱ��:2009-9-15
** ��һ���޸�ʱ��:��
**************************************************************/
static void CS5463_GetPactiveRMS(void)
{
 float G = 1.0,result;
 uchar temp,i,j;

 CS5463ReadReg(0x14,RX_Buff);   	//��ȡ�й�����REG_Pactive
 //SndCom1Data(RX_Buff,3);
 temp = RX_Buff[0];
 if(temp&0x80)						  	//���Ϊ����������ԭ��
 {
  	RX_Buff[0] = ~RX_Buff[0];			//����Ϊȡ��+1��������Ϊ���ȵ�ԭ�򣬲�+1
	RX_Buff[1] = ~RX_Buff[1];
	RX_Buff[2] = ~RX_Buff[2];		 	
 }
 i = 0;
 result = 0;
 while(i<3)
 {
  	temp = RX_Buff[i];			   		
	j = 0;
	while(j<8) //PT2017-7-7	    ԭ8 
	{
	 	if(temp&0x80)	 //ԭ0x80
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
** ��������:CS5463_GetPowerFactor
** ��������:��ȡ������������
** ��������:��
** ����ʱ��:2009-11-02
** ��һ���޸�ʱ��:��
**************************************************************/
static void CS5463_GetPowerFactor(void)
{
 float G = 1.0,result;
 uchar temp,i,j;

 CS5463ReadReg(0x32,RX_Buff);   		//��ȡ��������
 //SndCom1Data(RX_Buff,3);
 temp = RX_Buff[0];
 if(temp&0x80)						  	//���Ϊ����������ԭ��
 {
  	RX_Buff[0] = ~RX_Buff[0];			//����Ϊȡ��+1��������Ϊ���ȵ�ԭ�򣬲�+1
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
** ��������:Sendata(Udatlen)	 
** ��������: ���ͻ�Ӧ��Ϣ����
** ��������: ���͸���
** ����ʱ��:2017-7-3
** ��һ���޸�ʱ��:��
**************************************************************/

void Sendata(uchar ord,uchar Udatlen,uchar *PP)	 //������,���͵ĸ���
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
	   EPB.PB_dat[1]=x24c02_read(11);	//�ǵĹ�12�� 



//     VB= x24c02_read(2)*256+	x24c02_read(3);
//     IK= x24c02_read(4)*256+	x24c02_read(5);
//	   IB= x24c02_read(6)*256+	x24c02_read(7);
//	   PK= x24c02_read(8)*256+	x24c02_read(9);
//	   PB= x24c02_read(10)*256+	x24c02_read(11);

}




/*************************************************************************************************
** main������
*************************************************************************************************/
void main()
{
    
	com_init();             //���ڳ�ʼ��
    x24c02_init();          //��ʼ��24C02
	CS5463_Init();
	uDelay(200);
	ReadEVIP();
    while(1)
    {
		sta	= CS5463_GetStatusReg();		  	//����жϲ�����ԭ��
		if(0x01==(sta&0x01))		   			//��ȡ������ѹ
		{	

			CS5463_ResetStatusReg();			//�����־
			CS5463_GetVoltRMS();				//��ȡ��ѹ
			CS5463_GetCurrentRMS();				//��ȡ����
			CS5463_GetPactiveRMS();				//��ȡ����
			CS5463_GetPowerFactor();			//��ȡ��������
		}

		   else if(0x02==(sta&0x02))			//��ȡ�¶�
			{	
		//	CS5463_GetVoltRMS();				//��ȡ��ѹ								
		//	CS5463_GetTemperature();		    //�¶ȶ�ȡ����Ҫ̫Ƶ�������Ը�������ѹһ���ȡ

			//CS5463_Init();				//���³�ʼ��оƬ
			}



        if(flag==1)
        {
            ES=0;
            flag=0;
            switch(VAPELdata[2])
            {
            case 0:    //��12��	��ѹ ���� ���� KB ��12���ֽ�  Ϊʲôû���ݷ��أ� �ǲ�����FB FB
                for(ii=0; ii<12; ii++)
                {
                    EE12data[ii]=x24c02_read(ii);
                   
                }
				Sendata(VAPELdata[2],12,&EE12data);	 //��Ӧ���ݵ������� VAPELdata[2]
                break;
           
            case 1:    //��ѹУ׼	 VK VB  д��EE Ϊʲô��������ԣ�     FB FB 02 04 02 71 FF CE 3C FE FE
               	 
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
	        case 2:    //������ 4 5 6 7
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

		    case 3:    //������ 8 9 10 11
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

			  case 4:	//��ѹ��ʾ  Ҫʵ��y=Kx+b
				
				CS5463_GetVoltRMS();  //����float���ͱ����� K BҪ�ŵ����������
				
                Sendata(VAPELdata[2],4,&CS5463_Vrms.Vrms_suc.Vrms_dat);	  //��������Ǵ��ڷ�������

                   
                   
                
			  break;

			  case 5:   //����
				CS5463_GetCurrentRMS();
			    Sendata(VAPELdata[2],4,&CS5463_Irms.Irms_suc.Irms_dat);	
			  break;
			
			  case 6:   //����
				CS5463_GetPactiveRMS();
			    Sendata(VAPELdata[2],4,&CS5463_Pow.Pow_suc.Pow_dat);	
			  break;  // 

			   case 7:   //�Ƿ�Ҫ��һ������ͬʱ��3������ ��ѹ �������� ��������
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
void ser() interrupt 4	//�����ж�  ���ж�Э������ж�����
{
   	if(RI)	    //����ǽ����ж�
	{
	  RI=0;
	 // P2=SBUF;
	  indata[inCNT]=SBUF;      //��Ҫ����һ����������Ž��յ����� indata[] 	
	   inCNT++; 

	   if(inCNT>=6)	//�Ͱѳ��ȵ��ֽڶ�������
	   {	
	      if((indata[0]==0xFB)&&(indata[1]==0xFB))
		  {

			   if(inCNT>=(indata[3]+7))
			   {
			      sum=0;
			   	  for(ii=0;ii<indata[3]+4;ii++)	  //������У�� �Ͱ�β 
				  {
				  	sum=sum+ indata[ii];
				  
				  }


				  if((indata[indata[3]+4]==sum)&&(indata[indata[3]+5]==0xFE)&&(indata[indata[3]+6]==0xFE))
				  {

							 for(ii=0;ii<indata[3]+7;ii++)
							 {	
							   VAPELdata[ii]=indata[ii];  //����ȫ������

							  }

				  		  flag=1;	   //�����־ Ҫ�Ǽ���ͷ ���������� �Ͱ�β��ͬ���� 
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
