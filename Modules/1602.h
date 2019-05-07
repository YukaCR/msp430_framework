#include "usci_i2c_hw.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
unsigned char _Addr = 0, _displayfunction = 0, _displaycontrol = 0, _displaymode = 0, _numlines = 0,
              _cols = 0, _rows = 0, _backlightval = 0;
void __init__(uint8_t LCD_Addr, uint8_t LCD_cols, uint8_t LCD_rows);
void begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
void clear();
void home();
void ftoa(float f, char *str, uint8_t precision);
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
void printInt(uint32_t num);
void printFloat(double num, uint8_t acc);
char *itoa(uint32_t num, char *str, int radix);
void Delay1ms();
uint8_t powInt(uint8_t x, uint8_t y);
void ftoa(float f, char *str, uint8_t precision) {
  uint8_t i, j, divisor = 1;
  int8_t log_f;
  int32_t int_digits = (int)f;             //store the integer digits
  float decimals;
  char s1[12];

  memset(str, 0, sizeof(s1));
  memset(s1, 0, 10);

  if (f < 0) {                             //if a negative number
    str[0] = '-';                          //start the char array with '-'
    f = abs(f);                            //store its positive absolute value
  }
  log_f = ceil(log10(f));                  //get number of digits before the decimal
  if (log_f > 0) {                         //log value > 0 indicates a number > 1
    if (log_f == precision) {              //if number of digits = significant figures
      f += 0.5;                            //add 0.5 to round up decimals >= 0.5
      itoa(f, s1, 10);                     //itoa converts the number to a char array
      strcat(str, s1);                     //add to the number string
    }
    else if ((log_f - precision) > 0) {    //if more integer digits than significant digits
      i = log_f - precision;               //count digits to discard
      divisor = 10;
      for (j = 0; j < i; j++) divisor *= 10;    //divisor isolates our desired integer digits
      f /= divisor;                             //divide
      f += 0.5;                            //round when converting to int
      int_digits = (int)f;
      int_digits *= divisor;               //and multiply back to the adjusted value
      itoa(int_digits, s1, 10);
      strcat(str, s1);
    }
    else {                                 //if more precision specified than integer digits,
      itoa(int_digits, s1, 10);            //convert
      strcat(str, s1);                     //and append
    }
  }

  else {                                   //decimal fractions between 0 and 1: leading 0
    s1[0] = '0';
    strcat(str, s1);
  }

  if (log_f < precision) {                 //if precision exceeds number of integer digits,
    decimals = f - (int)f;                 //get decimal value as float
    strcat(str, ".");                      //append decimal point to char array

    i = precision - log_f;                 //number of decimals to read
    for (j = 0; j < i; j++) {              //for each,
      decimals *= 10;                      //multiply decimals by 10
      if (j == (i-1)) decimals += 0.5;     //and if it's the last, add 0.5 to round it
      itoa((int)decimals, s1, 10);         //convert as integer to character array
      strcat(str, s1);                     //append to string
      decimals -= (int)decimals;           //and remove, moving to the next
    }
  }
}

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
    i2c_send_init(LCD_Addr,40);
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
void printInt(uint32_t num){
	char stdD[10];
	printStr(itoa(num,stdD,10));
}
void printFloat(double num, uint8_t acc)
{
    char ftoap[10] = "";
    ftoa(num,ftoap,acc);
    printStr(ftoap);
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
char *itoa(uint32_t num, char *str, int radix)
{ /*绱㈠紩琛�*/
    char index[] = "0123456789ABCDEF";
    unsigned unum; /*涓棿鍙橀噺*/
    char temp;
    int i = 0, j, k;
    /*纭畾unum鐨勫��*/
    if (radix == 10 && num < 0) /*鍗佽繘鍒惰礋鏁�*/
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
        unum = (unsigned)num; /*鍏朵粬鎯呭喌*/
    /*杞崲*/
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    /*閫嗗簭*/
    if (str[0] == '-')
        k = 1; /*鍗佽繘鍒惰礋鏁�*/
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
