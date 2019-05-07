#ifndef __OLED_H__
#define __OLED_H__
#include "usci_b_i2c.h"
#include "oledfont.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#define OLED_ADDRESS 0x3c
const uint8_t oled_init_data[29]={0XAE,0X00,0X10,0X40,0X81,
		0XCF,0XA1,0XC8,0XA6,0XA8,0X3F,
		0XD3,0X00,0XD5,0X80,0XD9,0XF1,
		0XDA,0X12,0XDB,0X40,0X20,0X02,
		0X8D,0X14,0XA4,0XA6,0XAF,0XAF};
uint8_t current_font = 1;
uint8_t x = 0;
uint8_t	y = 0;
template <typename T>
char *itoa(T num, char *str, int radix )
{
	const char index[] = "0123456789ABCDEF";
	T unum;
	int	i = 0, j, k;
	if ( radix == 10 && num < 0 )
	{
		unum		= (T) -num;
		str[i++]	= '-';
	}else unum = (T) num;
	do
	{
		str[i++]	= index[(long)unum % radix];
		unum		/= radix;
	}
	while ( unum );
	str[i] = '\0';
	if ( str[0] == '-' )
		k = 1;
	else k = 0;
	char temp;
	for ( j = k; j <= (i - 1) / 2; j++ )
	{
		temp			= str[j];
		str[j]			= str[i - 1 + k - j];
		str[i - 1 + k - j]	= temp;
	}
	return(str);
}
void oled_write(uint8_t data);
void oled_write(const uint8_t *buffer, size_t size);
void oled_write(const uint8_t *buffer, uint8_t size);
void oled_write(const char *buffer, uint8_t size);
inline void oled_write(int8_t *buffer, size_t size);
inline void oled_write(char *buffer, uint8_t size);
void OLED_setXY(uint8_t _x,uint8_t _y);
void ftoa(float f, char *str, uint8_t precision);
void OLED_fill(uint8_t data);
template <typename T>
inline void OLED_printAt(T buffer,uint8_t _x,uint8_t _y,uint8_t size){
    OLED_setXY(_x,_y)
    return oled_write(T,size)
}
void OLED_init();
int i = 0;
void oled_print(const char* data){
    uint8_t length;
    length = strlen(data);
    oled_write(data,length);
}
void oled_println(const char* data){
    oled_print(data);
    oled_write('\n');
}
void oled_print(const double data,const uint8_t precision = 2){
    int max_data=0;
    char* buffer;
    uint8_t max_length = 0;
    max_data+= data;
    while(max_data){
        max_data /= 10;
        max_length ++;
    }
    max_length ++;//.
    if(data<0)max_length++;//-
    max_length += precision;//dot
    buffer = (char*)malloc(sizeof(char)*max_length);
    ftoa(data,buffer,precision);
    oled_write(buffer,max_length);
    free(buffer);
}
inline void oled_print(const float data,const uint8_t precision = 2){
    return oled_print((double)data,precision);
}
template<typename T>
void oled_print_number(T num){
    char* data;
    T max_data = num; 
    uint8_t max_length=0;
    while(max_data){
        max_length ++;
        max_data /= 10;
    }
    data = (char*)malloc(sizeof(char)*max_length);
    //delete max_length;
    itoa(num,data,10);
    oled_write(data,max_length);
    free(data);
}

inline void oled_write(char *buffer, uint8_t size){
    return oled_write((uint8_t*)buffer,size);
}
inline void oled_write(int8_t *buffer, uint8_t size){
    return oled_write((uint8_t*)buffer,size);
}
void oled_write(const uint8_t *buffer, uint8_t size){
	uint8_t i;
	for(i = 0;i<size;i++){
        oled_write(buffer[i]);
    }
}
void oled_write(const char *buffer, uint8_t size){
	uint8_t i;
	for(i = 0;i<size;i++){
        oled_write(buffer[i]);
    }
}
void oled_write(uint16_t number,uint16_t radix){
    char* data;
    data = (char*)malloc(sizeof(char)*10);
    itoa(number,data,radix);
    oled_write(data,strlen(data));
    free(data);
}
void oled_write(double number,uint16_t accu){
    char* data;
    data = (char*)malloc(sizeof(char)*(10+accu));
    ftoa(number,data,accu);
    oled_write(data,strlen(data));
    free(data);
}
void oled_write(uint8_t *buffer, uint8_t size){
	uint8_t i;
	for(i = 0;i<size;i++){
        oled_write(buffer[i]);
    }
}
void oled_write(uint8_t data){
    if(current_font!=2){
        if(data >= 32 && data <= (uint8_t)'z'){
            if(current_font){
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)F8X16[data - 32],8,5);
                OLED_setXY(x,y+1);
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(F8X16[data - 32] + 8),8,5);
                OLED_setXY(x+8,y-1);
            }
            else{
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)F6x8[data - 32],6,5);
                x=x+6;
            }
        }
        else if(data == '\n'){
            OLED_setXY(0,y+1+current_font);
        }
    }
    else{
        if(data!='\n'){
            i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(Hzk[data]),16,5);
            OLED_setXY(x,y+1);
            i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(Hzk[data]+16),16,5);
            OLED_setXY(x>=112?x+16:0,y);
        }
        else{
            OLED_setXY(0,y>=6?0:y+2);
        }
    }
    return;
}
void OLED_setXY(uint8_t _x,uint8_t _y){
    uint8_t set_posion[3];
    set_posion[0] = 0xb0+_y;
    set_posion[1] = ((_x&0xf0)>>4)|0x10;
    set_posion[2] = (_x&0x0f)|0x01;
    i2c_write_block(OLED_ADDRESS,0x00,set_posion,3,5);
    x = _x;
    y = _y;
};
void OLED_fill(uint8_t data){
    uint8_t cmd[3]={0xb0,0x00,0x10};
	for(uint8_t i=0;i<8;i++)
	{
        cmd[0] = 0xb0 + i;
        i2c_write_block(OLED_ADDRESS,0x00,cmd,3,5);
		for(uint8_t n=0;n<128;n++)i2c_write_block(OLED_ADDRESS,0x40,data,5);
	}
};
void OLED_init(){
    i2c_master_init();
    i2c_write_block(OLED_ADDRESS,0x00,(uint8_t*)oled_init_data,29,5);
    OLED_setXY(0,0);
}
uint16_t fast_pow(uint8_t x, uint8_t y){
    uint16_t result = x;
    while(y--){
        result *= x;
    }
    return result;
}
/*
char* ftoa(double num,char* data,int accu){
    uint8_t lower = 0,head_len;
    itoa(floor(num),data,10);
    head_len = strlen(data);
    num -= floor(num);
    data[head_len+1] = '.';
    while(num<0.1 && num){
        data[lower+head_len+1] = '0';
        lower ++;
        num *= 10;
    }
    num*=fast_pow(10,accu-lower);
    itoa(floor(num),data+head_len+lower+1,10);
    return data;
}*/
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


#endif
