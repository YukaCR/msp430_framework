/*
*	Author: YukaCR
*	sukoi no TFT driver for msp430.
*	self method for gfx without buffer.
*/
#pragma once
#include "HW_SPI.h"
/*
sbit SCL = P1 ^ 0;
sbit RST_TFT = P1 ^ 1;
sbit CS1 = P1 ^ 6;
sbit MISO = P1 ^ 7;

sbit SDIN = P5 ^ 4;
sbit DC = P5 ^ 5;
sbit BLK = P3 ^ 7;
sbit CS2 = P3 ^ 6;
 */
#define OLED_RST_GPIO	P4OUT
#define OLED_RST_Port	BIT2
#define OLED_DC_GPIO	P3OUT
#define OLED_DC_Port	BIT5
#define OLED_CS_GPIO	P4OUT
#define OLED_CS_Port	BIT1
#define Font_CS_GPIO	P3OUT
#define Font_CS_Port	BIT2

#define DMA1_IN_USE
void LCD_DMA_Init(){
	DMACTL0 |= DMA1TSEL__USCIA0TX;
    DMACTL4 = DMARMWDIS;
    __data20_write_long((uintptr_t)&DMA1DA, (uintptr_t)&UCA0TXBUF);
}
void LCD_GPIO_Init(){
	P4SEL &=~ BIT2 | BIT1;
	P4OUT |= BIT2 | BIT1;
	P4DIR |= BIT2 | BIT1;
	P4REN |= BIT2 | BIT1;
	P3SEL &=~ BIT5 | BIT1;
	P3OUT |= BIT5 | BIT1;
	P3DIR |= BIT5 | BIT1;
	P3REN |= BIT5 | BIT1;
}

#define OLED_RST_TFT_Set OLED_RST_GPIO |= OLED_RST_Port;
#define OLED_RST_TFT_Clr OLED_RST_GPIO &=~ OLED_RST_Port;

#define OLED_DC_Set OLED_DC_GPIO |= OLED_DC_Port;
#define OLED_DC_Clr OLED_DC_GPIO &=~ OLED_DC_Port;

#define OLED_CS_Set OLED_CS_GPIO |= OLED_CS_Port;
#define OLED_CS_Clr OLED_CS_GPIO &=~ OLED_CS_Port;

#define ZK_CS_Set Font_CS_GPIO |= Font_CS_Port;
#define ZK_CS_Clr Font_CS_GPIO &=~ Font_CS_Port;

#define LCD_W 240
#define LCD_H 240
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define OLED_CMD 0
#define OLED_DATA 1
#define OLED_MODE 0
#define SIZE 16
#define Brightness 0xFF
extern u16 BACK_COLOR, POINT_COLOR;
void delay_ms(unsigned int ms);
void LCD_Writ_Bus(u8 dat);
void LCD_WR_DATA8(u8 dat);
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u8 dat);
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);
void Lcd_Init(void);
void LCD_Clear(u16 Color);
void LCD_DrawPoint(u16 x, u16 y, u16 color);
void LCD_DrawPoint_big(u16 x, u16 y, u16 colory);
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);
u32 mypow(u8 m, u8 n);
void LCD_ShowPicture(u16 x1, u16 y1, u16 x2, u16 y2);

void CL_Mem(void);
void ZK_command(u8 dat);
u8 get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(u8 AddrHigh, u8 AddrMid, u8 AddrLow, u8 *pBuff, u8 DataLen);
void Display_GB2312(u16 x, u16 y, u8 zk_num, u16 color);
void Display_GB2312_String(u16 x, u16 y, u8 zk_num, u8 text[], u16 color);
void Display_Asc(u16 x, u16 y, u8 zk_num, u16 color);
void Display_Asc_String(u16 x, u8 y, u16 zk_num, u8 text[], u16 color);

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
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430

#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458

#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12
u16 BACK_COLOR;
inline void delay_ms(unsigned int ms)
{
	while(ms--){
		__delay_cycles(1000 * 25);
	}
}
inline void LCD_Writ_Bus(u8 dat)
{
	OLED_CS_Clr;
	spi_send(dat);
	OLED_CS_Set;
}

inline void LCD_WR_DATA8(u8 dat)
{
	OLED_DC_Set;
	LCD_Writ_Bus(dat);
}

inline void LCD_WR_DATA(u16 dat)
{
	OLED_DC_Set;
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}

inline void LCD_WR_REG(u8 dat)
{
	OLED_DC_Clr;
	LCD_Writ_Bus(dat);
}

inline void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);
}

void Lcd_Init(void)
{
	Setup_SPI_Master();
	SPI_SetCPOL_CPHA(false,false);
	LCD_GPIO_Init();
	LCD_DMA_Init();
	OLED_RST_TFT_Clr;
	delay_ms(20);
	OLED_RST_TFT_Set;
	delay_ms(20);

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
	LCD_WR_REG(120);

	LCD_WR_REG(0x29);
}

void LCD_Clear(u16 unknown)// 40MHz SPI , 26.76ms
{
	volatile uint8_t kpdata = unknown;// clear AS black;
	LCD_Address_Set(0,0,239,239);
	OLED_CS_Clr;
    OLED_DC_Set;
	UCA0IFG &=~ UCTXIFG;
	__data20_write_long((uintptr_t)&DMA1SA, (uintptr_t)&kpdata);
    DMA1CTL = DMAEN | DMASRCBYTE | DMADSTBYTE;
	DMA1SZ = 2;
	UCA0IFG |= UCTXIFG; 
	while(!(DMA1CTL & DMAIFG));

    LCD_Address_Set(0,0,239,239);
    OLED_CS_Clr;
    OLED_DC_Set;
    DMA1SZ = 57600;
    DMA1CTL = DMAEN | DMASRCBYTE | DMADSTBYTE;
    UCA0IFG &=~ UCTXIFG;
    UCA0IFG |= UCTXIFG;
    while(!(DMA1CTL & DMAIFG));
    DMA1SZ = 57600;
    DMA1CTL = DMAEN | DMASRCBYTE | DMADSTBYTE;
    UCA0IFG &=~ UCTXIFG;
    UCA0IFG |= UCTXIFG;
    while(!(DMA1CTL & DMAIFG));
	OLED_CS_Set;
}

void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
	LCD_Address_Set(x, y, x, y);
	LCD_WR_DATA(color);
}

void LCD_DrawPoint_big(u16 x, u16 y, u16 color)
{
	LCD_Fill(x - 1, y - 1, x + 1, y + 1, color);
}

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
	u16 i, j;
	LCD_Address_Set(xsta, ysta, xend, yend);
	for (i = ysta; i <= yend; i++)
	{
		for (j = xsta; j <= xend; j++)
			LCD_WR_DATA(color);
	}
}

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1;
	else if (delta_x == 0)
		incx = 0;
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0;
	else
	{
		incy = -1;
		delta_y = -delta_x;
	}
	if (delta_x > delta_y)
		distance = delta_x;
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color);
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
		LCD_DrawPoint(x0 - b, y0 - a, color);
		LCD_DrawPoint(x0 + b, y0 - a, color);
		LCD_DrawPoint(x0 - a, y0 + b, color);
		LCD_DrawPoint(x0 - a, y0 - b, color);
		LCD_DrawPoint(x0 + b, y0 + a, color);
		LCD_DrawPoint(x0 + a, y0 - b, color);
		LCD_DrawPoint(x0 + a, y0 + b, color);
		LCD_DrawPoint(x0 - b, y0 + a, color);
		a++;
		if ((a * a + b * b) > (r * r))
		{
			b--;
		}
	}
}

u32 mypow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
		result *= m;
	return result;
}

u8 FontBuf[128];
void ZK_command(u8 dat)
{
	spi_send(dat);
}
u8 get_data_from_ROM(void)
{
	u8 ret_data;
	ret_data = spi_send(0x0f);
	return (ret_data);
}
void get_n_bytes_data_from_ROM(u8 AddrHigh, u8 AddrMid, u8 AddrLow, u8 *pBuff, u8 DataLen)
{
	u8 i;
	OLED_CS_Set;
	ZK_CS_Clr;

	ZK_command(0x03);
	ZK_command(AddrHigh);
	ZK_command(AddrMid);
	ZK_command(AddrLow);
	for (i = 0; i < DataLen; i++)
	{
		*(pBuff + i) = get_data_from_ROM();
	}

	ZK_CS_Set;
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
	u8 AddrHigh, AddrMid, AddrLow;

	u32 FontAddr = 0;
	u32 BaseAdd = 0;
	u8 n, h, w, d, p;
	switch (zk_num)
	{
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
		}

		if (((text[i] >= 0xA1) && (text[i] <= 0xA9)) && (text[i + 1] >= 0xA1))
		{
			FontAddr = (text[i] - 0xA1) * 94;
			FontAddr += (text[i + 1] - 0xA1);
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;
			AddrMid = (FontAddr & 0xff00) >> 8;
			AddrLow = FontAddr & 0xff;
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n);
			Display_GB2312(x, y, zk_num, color);
		}

		else if (((text[i] >= 0xB0) && (text[i] <= 0xF7)) && (text[i + 1] >= 0xA1))
		{

			FontAddr = (text[i] - 0xB0) * 94;
			FontAddr += (text[i + 1] - 0xA1) + 846;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;
			AddrMid = (FontAddr & 0xff00) >> 8;
			AddrLow = FontAddr & 0xff;
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n);
			Display_GB2312(x, y, zk_num, color);
		}
		x = x + d;
		i += 2;
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
	u8 AddrHigh, AddrMid, AddrLow;

	u32 FontAddr = 0;
	u32 BaseAdd = 0;
	u8 n, h, w, d, c;
	switch (zk_num)
	{
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
		if (x > (240 - d))
		{
			y = y + c;
			x = 0;
		}
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;
			AddrMid = (FontAddr & 0xff00) >> 8;
			AddrLow = FontAddr & 0xff;
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n);
			Display_Asc(x, y, zk_num, color);
		}
		i += 1;
		x += d;
	}
}
