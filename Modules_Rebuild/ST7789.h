#ifndef ST7789
#define ST7789

/*sbit SCL=P0^0;
sbit SDIN=P0^1;
sbit RST=P0^2;
sbit DC=P0^3;
sbit CS1=P0^4;
sbit BLK=P0^5;
sbit MISO=P0^6;
sbit CS2=P0^7; */
#include "SPI.h"
#include "ST7789_Font.h"
#define CS_Pin BIT5
#define DC_Pin BIT6
#define Reset_Pin BIT2
#define CS2_Pin BIT2

#define BIS_CS P3OUT |= CS_Pin
#define BIC_CS P3OUT &= ~CS_Pin
#define BIR_CS \
    BIS_CS;    \
    BIC_CS
#define BIS_Reset P3OUT |= Reset_Pin
#define BIC_Reset P3OUT &= ~Reset_Pin
#define BIS_DC P3OUT |= DC_Pin
#define BIC_DC P3OUT &= ~DC_Pin
#define BIS_CS2 P8OUT |= CS2_Pin
#define BIC_CS2 P8OUT &= ~CS2_Pin
#define BIR_CS2 \
    BIS_CS2;    \
    BIC_CS2

#define LCD_Writ_Bus ST7789_Write_Bus
#define OLED_RST_Set() BIS_Reset;
#define OLED_RST_Clr() BIC_Reset;
#define OLED_DC_Set() BIS_DC;
#define OLED_DC_Clr() BIC_DC;
#define OLED_CS_Clr() BIC_CS;
#define OLED_CS_Set() BIS_CS;
#define ZK_CS_Clr() BIC_CS2;
#define ZK_CS_Set() BIS_CS2;
#define LCD_WR_DATA8(x) LCD_WR_DATA((uint8_t)x)
#define delay_ms(x) __delay_cycles(25 * 1000 * x)

#define FPS 60
#define Timer_Peroid 4000000 / FPS
#define splash 1

#define LCD_W 240
#define LCD_H 240
#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据
#define OLED_MODE 0
#define SIZE 16
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

//画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //棕色
#define BRRED 0XFC07 //棕红色
#define GRAY 0X8430  //灰色
//GUI颜色

#define DARKBLUE 0X01CF  //深蓝色
#define LIGHTBLUE 0X7D7C //浅蓝色
#define GRAYBLUE 0X5458  //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F //浅绿色
#define LGRAY 0XC618      //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12    //浅棕蓝色(选择条目的反色)

u16 BACK_COLOR, POINT_COLOR;
u8 FontBuf[128]; //字库缓存
template <typename T_t>
inline void LCD_Writ_Bus(T_t dat)
{
    OLED_CS_Clr();
    spi_send(dat);
    OLED_CS_Set();
}

inline void LCD_WR_DATA(uint8_t dat)
{
    OLED_DC_Set();
    LCD_Writ_Bus(dat);
}

inline void LCD_WR_DATA(uint16_t dat)
{
    OLED_DC_Set();
    LCD_Writ_Bus(dat);
}
inline void LCD_WR_REG(uint8_t dat)
{
    OLED_DC_Clr(); //写命令
    LCD_Writ_Bus(dat);
}

void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_WR_REG(0x2a); //列地址设置
    LCD_WR_DATA(x1);
    LCD_WR_DATA(x2);
    LCD_WR_REG(0x2b); //行地址设置
    LCD_WR_DATA(y1);
    LCD_WR_DATA(y2);
    LCD_WR_REG(0x2c); //储存器写
}
void OLED_BLK_Set()
{
    P2SEL |= BIT0;
    P2DIR |= BIT0;
    P2DS |= BIT0;
    TA1CTL = TASSEL__SMCLK + ID_0 + MC__STOP;
    TA1CCR0 = (uint16_t)(Timer_Peroid);
    TA1CCR1 = (uint16_t)(TA1CCR0 >> 1);
    TA1CCTL0 = CM_1 + SCS + SCCI + CAP;
    TA1CCTL1 = CM_0 + SCS + OUTMOD_6;
    TA1CTL |= MC__UP;
}
void Lcd_Init(void)
{
    Setup_SPI();
    OLED_RST_Clr();
    __delay_cycles(25000);
    OLED_RST_Set();
    __delay_cycles(25000);
    OLED_BLK_Set();

    //************* Start Initial Sequence **********//
    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x12);

    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xD0);
    LCD_WR_DATA8(0xA4);
    LCD_WR_DATA8(0xA1);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21);

    LCD_WR_REG(0x11);
    //Delay (120);

    LCD_WR_REG(0x29);
}
void LCD_Clear(u16 Color)
{
    u16 i, j;
    LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
    for (i = 0; i < LCD_W; i++)
    {
        for (j = 0; j < LCD_H; j++)
        {
            LCD_WR_DATA(Color);
        }
    }
}

void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
    LCD_Address_Set(x, y, x, y); //设置光标位置
    LCD_WR_DATA(color);
}
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i, j;
    LCD_Address_Set(xsta, ysta, xend, yend); //设置光标位置
    for (i = ysta; i <= yend; i++)
    {
        for (j = xsta; j <= xend; j++)
            LCD_WR_DATA(color); //设置光标位置
    }
}

void LCD_DrawPoint_big(u16 x, u16 y, u16 color)
{
    LCD_Fill(x - 1, y - 1, x + 1, y + 1, color);
}
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; //画线起点坐标
    uCol = y1;
    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_x;
    }
    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        LCD_DrawPoint(uRow, uCol, color); //画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b)
    {
        LCD_DrawPoint(x0 - b, y0 - a, color); //3
        LCD_DrawPoint(x0 + b, y0 - a, color); //0
        LCD_DrawPoint(x0 - a, y0 + b, color); //1
        LCD_DrawPoint(x0 - a, y0 - b, color); //2
        LCD_DrawPoint(x0 + b, y0 + a, color); //4
        LCD_DrawPoint(x0 + a, y0 - b, color); //5
        LCD_DrawPoint(x0 + a, y0 + b, color); //6
        LCD_DrawPoint(x0 - b, y0 + a, color); //7
        a++;
        if ((a * a + b * b) > (r * r)) //判断要画的点是否过远
        {
            b--;
        }
    }
}
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode, u16 color)
{
    u8 temp;
    u8 pos, t;
    u16 x0 = x;
    if (x > LCD_W - 16 || y > LCD_H - 16)
        return;                                   //设置窗口
    num = num - ' ';                              //得到偏移后的值
    LCD_Address_Set(x, y, x + 8 - 1, y + 16 - 1); //设置光标位置
    if (!mode)                                    //非叠加方式
    {
        for (pos = 0; pos < 16; pos++)
        {
            temp = asc2_1608[(u16)num * 16 + pos]; //调用1608字体
            for (t = 0; t < 8; t++)
            {
                if (temp & 0x01)
                    LCD_WR_DATA(color);
                else
                    LCD_WR_DATA(BACK_COLOR);
                temp >>= 1;
                x++;
            }
            x = x0;
            y++;
        }
    }
    else //叠加方式
    {
        for (pos = 0; pos < 16; pos++)
        {
            temp = asc2_1608[(u16)num * 16 + pos]; //调用1608字体
            for (t = 0; t < 8; t++)
            {
                if (temp & 0x01)
                    LCD_DrawPoint(x + t, y + pos, color); //画一个点
                temp >>= 1;
            }
        }
    }
}

void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 color)
{
    while (*p != '\0')
    {
        if (x > LCD_W - 16)
        {
            x = 0;
            y += 16;
        }
        if (y > LCD_H - 16)
        {
            y = x = 0;
            LCD_Clear(RED);
        }
        LCD_ShowChar(x, y, *p, 0, color);
        x += 8;
        p++;
    }
}
void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size1,u16 color)	
{  
	u8 i,j;
	u8 *temp,size2;
	if(size1==16){temp=(u8*)Hzk16;}//选择字号
	if(size1==32){temp=(u8*)Hzk32;}
  LCD_Address_Set(x,y,x+size1-1,y+size1-1); //设置一个汉字的区域
  size2=size1*size1/8;//一个汉字所占的字节
	temp+=index*size2;//写入的起始位置
	for(j=0;j<size2;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)//从数据的低位开始读
			{
				LCD_WR_DATA(color);//点亮
			}
			else
			{
				LCD_WR_DATA(BACK_COLOR);//不点亮
			}
		}
		temp++;
	 }
}
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

void LCD_ShowNum(u16 x, u16 y, u16 num, u8 len, u16 color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + 8 * t, y, ' ', 0, color);
                continue;
            }
            else
                enshow = 1;
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color);
    }
}

void LCD_ShowNum1(u16 x, u16 y, float num, u8 len, u16 color)
{
    u8 t, temp;
    u8 enshow = 0;
    u16 num1;
    num1 = num * 100;
    for (t = 0; t < len; t++)
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2))
        {
            LCD_ShowChar(x + 8 * (len - 2), y, '.', 0, color);
            t++;
            len += 1;
        }
        LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color);
    }
}

void LCD_ShowPicture(u16 x1, u16 y1, u16 x2, u16 y2, uint8_t *image)
{
    int i;
    LCD_Address_Set(x1, y1, x2, y2);
    for (i = 0; i < 1600; i++)
    {
        LCD_WR_DATA8(image[i * 2 + 1]);
        LCD_WR_DATA8(image[i * 2]);
    }
}
inline void ZK_command(u8 dat)
{
    spi_send(dat);
}
inline u8 get_data_from_ROM(void)
{
    return spi_send((uint8_t)0x00); //返回读出的一个字节
}
void get_n_bytes_data_from_ROM(u8 AddrHigh, u8 AddrMid, u8 AddrLow, u8 *pBuff, u8 DataLen)
{
    u8 i;
    OLED_CS_Set();
    ZK_CS_Clr();          //字库片选
    ZK_command(0x03);     //写指令
    ZK_command(AddrHigh); //写地址高字节
    ZK_command(AddrMid);  //写地址中字节
    ZK_command(AddrLow);  //写地址低字节
    for (i = 0; i < DataLen; i++)
    {
        *(pBuff + i) = get_data_from_ROM(); //读一个字节数据
    }
    ZK_CS_Set(); //取消字库片选
}
void Display_GB2312(u16 x, u16 y, u8 zk_num, u16 color)
{
    u8 i, k;
    switch (zk_num)
    {
    case 1:
    {
        LCD_Address_Set(x, y, x + 15, y + 11);
        for (i = 0; i < 24; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; // 12*12

    case 2:
    {
        LCD_Address_Set(x, y, x + 15, y + 15);
        for (i = 0; i < 32; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; // 15*16

    case 3:
    {
        LCD_Address_Set(x, y, x + 23, y + 23);
        for (i = 0; i < 72; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; // 24*24

    case 4:
    {
        LCD_Address_Set(x, y, x + 31, y + 31);
        for (i = 0; i < 128; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; // 32*32
    }
}
void Display_GB2312_String(u16 x, u16 y, u8 zk_num, u8 text[], u16 color)
{

    u8 i = 0;
    u8 AddrHigh, AddrMid, AddrLow; //字高、中、低地址

    u32 FontAddr = 0; //字地址
    u32 BaseAdd = 0;  //字库基地址
    u8 n, h, w, d, p; // 不同点阵字库的计算变量
    switch (zk_num)
    { // n个数，h：字高度，w：字宽度， d：字间距，c：页大小
    case 1:
        BaseAdd = 0x00;
        n = 24;
        h = 12;
        w = 12;
        d = 12;
        p = 2;
        break; // 12*12
    case 2:
        BaseAdd = 0x2C9D0;
        n = 32;
        h = 16;
        w = 16;
        d = 16;
        p = 2;
        break; // 15*16
    case 3:
        BaseAdd = 0x68190;
        n = 72;
        h = 24;
        w = 24;
        d = 24;
        p = 3;
        break; // 24*24
    case 4:
        BaseAdd = 0xEDF00;
        n = 128;
        h = 32;
        w = 32;
        d = 32;
        p = 4;
        break; // 32*32
    }

    while ((text[i] > 0x00))
    {
        if (x > (240 - d))
        {
            y = y + p;
            x = 0;
        } //溢出换行

        if (((text[i] >= 0xA1) && (text[i] <= 0xA9)) && (text[i + 1] >= 0xA1))
        {
            //国标简体（GB2312）汉字在 字库IC中的地址由以下公式来计算：//
            //Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; 分三部取地址///
            FontAddr = (text[i] - 0xA1) * 94;
            FontAddr += (text[i + 1] - 0xA1);
            FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

            AddrHigh = (FontAddr & 0xff0000) >> 16;                            //地址的高8位,共24位//
            AddrMid = (FontAddr & 0xff00) >> 8;                                //地址的中8位,共24位//
            AddrLow = FontAddr & 0xff;                                         //地址的低8位,共24位//
            get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); //取一个汉字的数据，存到"FontBuf[]"
            Display_GB2312(x, y, zk_num, color);                               //显示一个汉字到OLED上/
        }

        else if (((text[i] >= 0xB0) && (text[i] <= 0xF7)) && (text[i + 1] >= 0xA1))
        {
            //国标简体（GB2312） 字库IC中的地址由以下公式来计算：//
            //Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; 分三部取地址//
            FontAddr = (text[i] - 0xB0) * 94;
            FontAddr += (text[i + 1] - 0xA1) + 846;
            FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

            AddrHigh = (FontAddr & 0xff0000) >> 16;                            //地址的高8位,共24位//
            AddrMid = (FontAddr & 0xff00) >> 8;                                //地址的中8位,共24位//
            AddrLow = FontAddr & 0xff;                                         //地址的低8位,共24位//
            get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); //取一个汉字的数据，存到"FontBuf[ ]"
            Display_GB2312(x, y, zk_num, color);                               //显示一个汉字到LCD上/
        }
        x = x + d; //下一个字坐标
        i += 2;    //下个字符
    }
}
void Display_Asc(u16 x, u16 y, u8 zk_num, u16 color)
{

    unsigned char i, k;

    switch (zk_num)
    {
    case 1:
    {
        LCD_Address_Set(x, y, x + 7, y + 7);
        for (i = 0; i < 7; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //	  5x7 ASCII

    case 2:
    {
        LCD_Address_Set(x, y, x + 7, y + 7);
        for (i = 0; i < 8; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //	  7x8 ASCII

    case 3:
    {
        LCD_Address_Set(x, y, x + 7, y + 11);
        for (i = 0; i < 12; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //  6x12 ASCII

    case 4:
    {
        LCD_Address_Set(x, y, x + 7, y + 15);
        for (i = 0; i < 16; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //  8x16 ASCII

    case 5:
    {
        LCD_Address_Set(x, y, x + 15, y + 24);
        for (i = 0; i < 48; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //  12x24 ASCII

    case 6:
    {
        LCD_Address_Set(x, y, x + 15, y + 31);
        for (i = 0; i < 64; i++)
        {
            for (k = 0; k < 8; k++)
            {
                if ((FontBuf[i] & (0x80 >> k)) != 0)
                {
                    LCD_WR_DATA(color);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
        }
    }
    break; //  16x32 ASCII
    }
}
void Display_Asc_String(u16 x, u8 y, u16 zk_num, u8 text[], u16 color)
{
    u8 i = 0;
    u8 AddrHigh, AddrMid, AddrLow; //字高、中、低地址

    u32 FontAddr = 0; //字地址
    u32 BaseAdd = 0;  //字库基地址
    u8 n, h, w, d, c; // 不同点阵字库的计算变量
    switch (zk_num)
    { // n个数，h：字高度，w：字宽度， d：字间距，c：页大小
    case 1:
        BaseAdd = 0x1DDF80;
        n = 8;
        h = 7;
        w = 5;
        d = 6;
        c = 1;
        break; //	  5x7 ASCII
    case 2:
        BaseAdd = 0x1DE280;
        n = 8;
        h = 8;
        w = 7;
        d = 8;
        c = 1;
        break; //   7x8 ASCII
    case 3:
        BaseAdd = 0x1DBE00;
        n = 12;
        h = 12;
        w = 6;
        d = 6;
        c = 2;
        break; //  6x12 ASCII
    case 4:
        BaseAdd = 0x1DD780;
        n = 16;
        h = 16;
        w = 8;
        d = 8;
        c = 2;
        break; //  8x16 ASCII
    case 5:
        BaseAdd = 0x1DFF00;
        n = 48;
        h = 24;
        w = 12;
        d = 12;
        c = 3;
        break; //  12x24 ASCII
    case 6:
        BaseAdd = 0x1E5A50;
        n = 64;
        h = 32;
        w = 16;
        d = 16;
        c = 4;
        break; //  16x32 ASCII
    }

    while ((text[i] > 0x00))
    {
        if (x > (240 - d)) //溢出换行
        {
            y = y + c;
            x = 0;
        }
        if ((text[i] >= 0x20) && (text[i] <= 0x7E))
        {
            FontAddr = text[i] - 0x20;
            FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

            AddrHigh = (FontAddr & 0xff0000) >> 16;                            /*地址的高8位,共24位*/
            AddrMid = (FontAddr & 0xff00) >> 8;                                /*地址的中8位,共24位*/
            AddrLow = FontAddr & 0xff;                                         /*地址的低8位,共24位*/
            get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*取一个汉字的数据，存到"FontBuf[]"*/
            Display_Asc(x, y, zk_num, color);                                  /*显示一个ascii到OLED上 */
        }
        i += 1; //下个数据
        x += d; //下一个字坐标
    }
}
#endif
