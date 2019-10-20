/*
*   Author: YukaCR
*   SSD1306 SPI driver. no gfx.
*/
#include "SPI.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "oledfont_spi_hw.h"
const uint8_t current_font = 0;
uint8_t x = 0;
uint8_t y = 0;
inline void SSD1306_RST_Value(const bool value)
{
    if(value){
        P4OUT |= BIT2;
    }
    else{
        P4OUT &=~ BIT2;
    }
}
inline void SSD1306_DC_Value(const bool value)
{
    if(value){
        P4OUT |= BIT1;
    }
    else{
        P4OUT &=~ BIT1;
    }
}
inline void SSD1306_CS_Value(const bool value)
{
    if(value){
        P3OUT |= BIT5;
    }
    else{
        P3OUT &=~ BIT5;
    }
}
const uint8_t oled_init_data[] =
    {0XAE, 0X00, 0X10, 0X40, 0X81,
     0XCF, 0XA1, 0XC8, 0XA6, 0XA8, 0X3F,
     0XD3, 0X00, 0XD5, 0X80, 0XD9, 0XF1,
     0XDA, 0X12, 0XDB, 0X40, 0X20, 0X02,
     0X8D, 0X14, 0XA4, 0XA6, 0XAF, 0XAF,
     0x20, 0x00, 0xb0, 0x10, 0x01};
template <typename T>
char *itoa(T num, char *str, int radix)
{
    const char index[] = "0123456789ABCDEF";
    T unum;
    int i = 0, j, k;
    if (radix == 10 && num < 0)
    {
        unum = (T)-num;
        str[i++] = '-';
    }
    else
        unum = (T)num;
    do
    {
        str[i++] = index[(long)unum % radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    if (str[0] == '-')
        k = 1;
    else
        k = 0;
    char temp;
    for (j = k; j <= (i - 1) / 2; j++)
    {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return (str);
}
void ftoa(float f, char *str, uint8_t precision)
{
    uint8_t i, j, divisor = 1;
    int8_t log_f;
    int32_t int_digits = (int)f; //store the integer digits
    float decimals;
    char s1[12];

    memset(str, 0, sizeof(s1));
    memset(s1, 0, 10);

    if (f < 0)
    {                 //if a negative number
        str[0] = '-'; //start the char array with '-'
        f = abs(f);   //store its positive absolute value
    }
    log_f = ceil(log10(f)); //get number of digits before the decimal
    if (log_f > 0)
    { //log value > 0 indicates a number > 1
        if (log_f == precision)
        {                    //if number of digits = significant figures
            f += 0.5;        //add 0.5 to round up decimals >= 0.5
            itoa(f, s1, 10); //itoa converts the number to a char array
            strcat(str, s1); //add to the number string
        }
        else if ((log_f - precision) > 0)
        {                          //if more integer digits than significant digits
            i = log_f - precision; //count digits to discard
            divisor = 10;
            for (j = 0; j < i; j++)
                divisor *= 10; //divisor isolates our desired integer digits
            f /= divisor;      //divide
            f += 0.5;          //round when converting to int
            int_digits = (int)f;
            int_digits *= divisor; //and multiply back to the adjusted value
            itoa(int_digits, s1, 10);
            strcat(str, s1);
        }
        else
        {                             //if more precision specified than integer digits,
            itoa(int_digits, s1, 10); //convert
            strcat(str, s1);          //and append
        }
    }
    else
    { //decimal fractions between 0 and 1: leading 0
        s1[0] = '0';
        strcat(str, s1);
    }

    if (log_f < precision)
    {                          //if precision exceeds number of integer digits,
        decimals = f - (int)f; //get decimal value as float
        strcat(str, ".");      //append decimal point to char array

        i = precision - log_f; //number of decimals to read
        for (j = 0; j < i; j++)
        {                   //for each,
            decimals *= 10; //multiply decimals by 10
            if (j == (i - 1))
                decimals += 0.5;         //and if it's the last, add 0.5 to round it
            itoa((int)decimals, s1, 10); //convert as integer to character array
            strcat(str, s1);             //append to string
            decimals -= (int)decimals;   //and remove, moving to the next
        }
    }
}
void Setup_SSD1306()
{
    int count;
    P4DIR |= BIT1 | BIT2;
    P3DIR |= BIT5;
    P4OUT |= BIT1 | BIT2;
    P3OUT |= BIT5;
    P4REN |= BIT1 | BIT2;
    P3REN |= BIT5;
    SSD1306_RST_Value(0);
    __delay_cycles(1000);
    SSD1306_RST_Value(1);
    __delay_cycles(1000);
    Setup_SPI();
    SSD1306_DC_Value(0);
    SSD1306_CS_Value(0);
    for (count = 0; count < sizeof(oled_init_data); count++)
    {
        spi_send(oled_init_data[count]);
    }
    SSD1306_CS_Value(1);
}
void OLED_setXY(uint8_t _x, uint8_t _y)
{
    SSD1306_DC_Value(0);
    SSD1306_CS_Value(0);
    spi_send(0xb0 + _y);
    spi_send(((_x & 0xf0) >> 4) | 0x10);
    spi_send((_x & 0x0f) | 0x01);
    x = _x;
    y = _y;
    SSD1306_CS_Value(1);
};
void oled_write_char(uint8_t data){
    if(data >= 32 && data <= (uint8_t)'z'){
        if(current_font){
            SSD1306_DC_Value(1);
            SSD1306_CS_Value(0);
            spi_send((uint8_t*)F8X16[data - 32],8);
            OLED_setXY(x,y+1);
            SSD1306_DC_Value(1);
            SSD1306_CS_Value(0);
            spi_send((uint8_t*)(F8X16[data - 32] + 8), 8);
            OLED_setXY(x+8,y-1);
        }
        else{
            SSD1306_DC_Value(1);
            SSD1306_CS_Value(0);
            spi_send((uint8_t*)F6x8[data - 32],6);
            x=x+6;
            SSD1306_CS_Value(1);
        }
    }
    else if(data == '\n'){
        OLED_setXY(0,y+1+current_font);
    }
    return;
}
void SSD1306_Fill(register uint8_t data){
    uint16_t count = 128 * 64 / 8;
    SSD1306_DC_Value(1);
    SSD1306_CS_Value(0);

    while(count --){
        spi_send(data);
    }
    SSD1306_CS_Value(1);

}
void oled_write_string(char *buffer, uint8_t size)
{
    uint8_t i;
    for (i = 0; i < size; i++)
    {
        oled_write_char(buffer[i]);
    }
}
void oled_write_float(const double data, const uint8_t precision)
{
    int max_data = 0;
    char *buffer;
    uint8_t max_length = 0;
    max_data += data;
    while (max_data)
    {
        max_data /= 10;
        max_length++;
    }
    max_length++; //.
    if (data < 0)
        max_length++;        //-
    max_length += precision; //dot
    buffer = (char *)malloc(sizeof(char) * max_length);
    ftoa(data, buffer, precision);
    oled_write_string(buffer, max_length);
    free(buffer);
}
void oled_write_number(uint32_t num)
{
    char *data;
    uint32_t max_data = num;
    uint8_t max_length = 0;
    while (max_data)
    {
        max_length++;
        max_data /= 10;
    }
    data = (char *)malloc(sizeof(char) * max_length);
    //delete max_length;
    itoa(num, data, 10);
    oled_write_string(data, max_length);
    free(data);
}
