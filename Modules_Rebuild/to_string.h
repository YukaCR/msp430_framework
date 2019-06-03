#ifndef TO_STRING_H
#define TO_STRING_H
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
char* to_string(char data){
    static char p_data[2]={0,0};
    p_data[0]= data;
    return p_data;
}
inline char* to_string(const char* data){
    return (char*)data;
}
inline char* to_string(char* data){
    return data;
}
char* to_string(uint8_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        uint8_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(uint16_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        uint16_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(uint32_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        uint32_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(int8_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        int8_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(int16_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        int16_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(int32_t n, int8_t base = 10)
{
    static char buf[8 * sizeof(long) + 2]; // Assumes 8-bit chars plus zero byte.
    char *str= &buf[sizeof(buf) - 1];
    bool neg_flag = 0;
    *str = '\0';
    if (base == 0){return 0;}
    if (base < 2)base = 10;
    if (n < 0)
    {
        neg_flag = 1;
        n = -1 * n;
    }
    // prevent crash if called with base == 1
    do
    {
        int32_t m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);
    if(neg_flag){*--str = '-';};
    return str;
}
char* to_string(float, int8_t = 2);
inline char* to_string(double number, int8_t digits = 2){
    return to_string((float)number,digits);
}
char* to_string(float number, int8_t digits) 
{ 
  static char result[20];
  bool neg_flag = 0;
  char* str = result;
  // Handle negative numbers
  if (number < 0.0)
  {
     neg_flag = 1;
     number = -1*number;
  }
  // Round correctly so that print(1.999, 2) prints as "2.00"
  float rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)rounding/=10.0;
  number += rounding;
  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  float remainder = number - (float)int_part;
  char* int_part_str = to_string((int32_t)int_part);
  memcpy(str,int_part_str,strlen(int_part_str));
  str += strlen(int_part_str);
  // Print the decimal point, but only if there are digits beyond
  *str++ = '.';
  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    *str++ = (char)(toPrint + 48);
    remainder -= toPrint; 
  } 
  *str++ = '\0';
  return result;
}
#endif
