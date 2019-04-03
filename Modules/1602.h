#include "usci_i2c_hw.h"
unsigned char _Addr = 0, _displayfunction = 0, _displaycontrol = 0, _displaymode = 0, _numlines = 0,
              _cols = 0, _rows = 0, _backlightval = 0;
void __init__(uint8_t LCD_Addr, uint8_t LCD_cols, uint8_t LCD_rows);
void begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
void clear();
void home();
void noCursor();
void cursor();
void scrollDisplayLeft();
void display();
void scrollDisplayRight();
void rightToLeft();
void leftToRight();
void autoscroll();
void noAutoscroll();
void createChar(uint8_t location, uint8_t *charmap);
void setCursor(uint8_t col, uint8_t row);
void command(uint8_t value);
void init();
void setBacklight(uint8_t new_val);
void send(uint8_t value, uint8_t mode);
void write4bits(uint8_t value);
void expanderWrite(uint8_t _data);
void pulseEnable(uint8_t _data);
void sleep_us(uint8_t r);
void i2c_writeto(uint8_t, uint8_t);
void printStr(char *dstr);
void printChar(char ch);
void printInt(uint8_t num);
void printFloat(double num, char acc);
char *itoa(int num, char *str, int radix);
void Delay1ms();
uint8_t powInt(uint8_t x, uint8_t y);
inline void sleep_us(uint8_t r)
{
    while (r--)
    {
        __delay_cycles(18);
    }
}
inline void sleep_ms(uint8_t r)
{
    while (r--)
    {
        Delay1ms();
        r--;
    }
}
inline void Delay1ms() //@11.0592MHz
{
    __delay_cycles(25000);
}

void __init__(uint8_t LCD_Addr = 0x27, uint8_t LCD_cols = 16, uint8_t LCD_rows= 2)
{
    _Addr = LCD_Addr;
    _cols = LCD_cols;
    _rows = LCD_rows;
    _backlightval = 0x08;
    i2c_send_init(LCD_Addr,10);
}
void begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1)
    {
        _displayfunction |= 0x08;
    }
    _numlines = lines;
    if ((dotsize != 0) & (lines == 1))
    {
        _displayfunction |= 0x04;
    }
    sleep_ms(50);
    expanderWrite(_backlightval);
    sleep_ms(1000);
    write4bits(0x03 << 4);
    sleep_us(4500);
    write4bits(0x03 << 4);
    sleep_us(4500);
    write4bits(0x03 << 4);
    sleep_us(150);
    write4bits(0x02 << 4);
    command(0x20 | _displayfunction);
    _displaycontrol = 0x04 | 0 | 0;
    display();
    clear();
    _displaymode = 0x02 | 0;
    command(0x04 | _displaymode);
    home();
}
void clear()
{
    command(0x01);
    sleep_us(2000);
}
void home()
{
    command(0x02);
    sleep_us(2000);
}
void noCursor()
{
    _displaycontrol &= ~0x02;
    command(0x08 | _displaycontrol);
}
void cursor()
{
    _displaycontrol |= 0x02;
    command(0x08 | _displaycontrol);
}
void scrollDisplayLeft()
{
    command(0x10 | 0x08 | 0);
}
void display()
{
    _displaycontrol |= 0x04;
    command(0x08 | _displaycontrol);
}
void scrollDisplayRight()
{
    command(0x10 | 0x08 | 0x04);
}
void leftToRight()
{
    _displaymode |= 0x02;
    command(0x04 | _displaymode);
}
void rightToLeft()
{
    _displaymode &= ~0x02;
    command(0x04 | _displaymode);
}
void autoscroll()
{
    _displaymode |= 0x01;
    command(0x04 | _displaymode);
}
void noAutoscroll()
{
    _displaymode &= ~0x01;
    command(0x04 | _displaymode);
}
void createChar(uint8_t location, uint8_t charmap[])
{
    uint8_t i;
    location &= 0x07;
    command(0x40 | (location << 3));
    for (i = 0; i < 8; i++)
    {
        send(charmap[i], 0x01);
    }
}
void setCursor(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[4] = {0, 0x40, 0x14, 0x54};
    if (row > _numlines)
    {
        row = _numlines - 1;
    }
    command(0x80 | (col + row_offsets[row]));
}
void command(uint8_t value)
{
    send(value, 0);
}
void init()
{
    _displayfunction = 0 | 0 | 0;
    begin(_cols, _rows, 0);
}
void setBacklight(uint8_t new_val)
{
    if (new_val)
    {
        _backlightval = 0x08;
    }
    else
    {
        _backlightval = 0;
    }
    expanderWrite(0);
}
void send(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}
void write4bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}
void expanderWrite(uint8_t _data)
{
    i2c_writeto(_Addr, _data | _backlightval);
}
void pulseEnable(uint8_t _data)
{
    expanderWrite(_data | 0x04);
    sleep_us(1);
    expanderWrite(_data & ~0x04);
    sleep_us(50);
}
void i2c_writeto(uint8_t _Addr, uint8_t _data)
{
    i2c_send(1,&_data);
    while(NC);
}
void printStr(char *dstr)
{
    int i = 0;
    while (dstr[i] != '\0')
    {
        printChar(dstr[i]);
        i++;
    }
}
void printChar(char ch)
{
    send(ch, 0x01);
}
void printInt(uint8_t num)
{
    char std[5];
    printStr(itoa(num, std, 10));
}
void printFloat(double num, char acc)
{
    printInt((int)num);
    printInt('.');
    num -= (int)num;
    printInt((int)(num * powInt(10, acc)));
}
uint8_t powInt(uint8_t x, uint8_t y)
{
    while (y - 1)
    {
        x = x * x;
        y -= 1;
    }
    return x;
}
char *itoa(int num, char *str, int radix)
{ /*索引表*/
    char index[] = "0123456789ABCDEF";
    unsigned unum; /*中间变量*/
    char temp;
    int i = 0, j, k;
    /*确定unum的值*/
    if (radix == 10 && num < 0) /*十进制负数*/
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num; /*其他情况*/
    /*转换*/
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    /*逆序*/
    if (str[0] == '-')
        k = 1; /*十进制负数*/
    else
        k = 0;
    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}