/*
*   Author: YukaCR
*   sukoi no GFX with buffer.
*   example: main_DELETE.cpp program 0.
*/

#ifndef TempleGFX_h
#define TempleGFX_h
#include "../Modules/oledfont.h"
#include "Terminal3.h"
#include "stdlib.h"
#include <string.h>
#include "to_string.h"
uint8_t *GFXGraphicsMemory;
uint8_t Width, Height;
uint8_t GFXFont = 0;
bool GFX_Changed = 0;
struct Point
{
    uint8_t x, y;
} CursorPosition = {0, 0};
typedef Point Point_t;
void GFX_SetupMemory(uint8_t *_GraphicsMemory, uint8_t _Width, uint8_t _Height)
{
    GFXGraphicsMemory = _GraphicsMemory;
    Width = _Width;
    Height = _Height;
}
inline void GFX_SetFont(uint8_t GFXFontIndex) { GFXFont = GFXFontIndex; }
inline void GFX_SetPosition(uint8_t y, uint8_t x)
{
    CursorPosition.x = x;
    CursorPosition.y = y;
}
inline void GFX_CheckPosition()
{
    if (CursorPosition.x >= Width)
    {
        CursorPosition.x = 0;
        CursorPosition.y += 1;
    }
    if (CursorPosition.y >= (Height + 7) / 8)
    {
        CursorPosition.y = 0;
    }
}

void GFX_Fill(uint8_t data)
{
    GFX_Changed = 1;
    memset(GFXGraphicsMemory, data, (size_t)Width * (size_t)Height/8);
}
inline uint8_t *GetPositionPointer()
{
    return GFXGraphicsMemory + (CursorPosition.y + 1) * Width + CursorPosition.x;
}
inline uint8_t *GetPositionPointer(uint16_t x, uint8_t y)
{
    return GFXGraphicsMemory + y * Width + x;
}
inline uint8_t *GetPositionPointer(Point_t Position)
{
    return GFXGraphicsMemory + (Position.y + 1) * Width + Position.x;
}
void GFX_WriteChar_3(char data)
{
    if (data >= 32 && data <= (uint8_t)'~')
    {
        if (CursorPosition.y + 1 >= (Height + 7) / 8)
        {
            return;
        }
        memcpy(GFXGraphicsMemory + (CursorPosition.y + 1) * Width + CursorPosition.x, (Terminal3x5 - 32) + data, 3);
    }
    else if (data == '\n')
    {
        CursorPosition.y += 1;
        CursorPosition.x = 0;
        if (CursorPosition.y >= (Height + 7) / 8)
        {
            CursorPosition.y = 0;
        }
    }
}
void GFX_WriteChar(char data)
{
    GFX_Changed = 1;
    if (GFXFont != 2)
    {
        if (data >= 32 && data <= (uint8_t)'z')
        {
            if (GFXFont)
            {
                memcpy(GFXGraphicsMemory + CursorPosition.y * Width + CursorPosition.x, (F8X16 - 32) + data, 8);
                if (CursorPosition.y + 1 >= (Height + 7) / 8)
                {
                    return;
                }
                memcpy(GFXGraphicsMemory + (CursorPosition.y + 1) * Width + CursorPosition.x, (F8X16 - 32 + 8) + data, 8);
                CursorPosition.x += 8;
            }
            else
            {
                memcpy(GFXGraphicsMemory + CursorPosition.y * Width + CursorPosition.x, (F6x8 - 32) + data, 6);
                CursorPosition.x += 6;
            }
        }
        else if (data == '\n')
        {
            CursorPosition.y += 1;
            CursorPosition.x = 0;
            if (CursorPosition.y >= (Height + 7) / 8)
            {
                CursorPosition.y = 0;
            }
        }
    }
    else if (GFXFont != 3)
    {
        if (data != '\n')
        {
            memcpy(GFXGraphicsMemory + CursorPosition.y * Width + CursorPosition.x, Hzk + data, 16);
            if (CursorPosition.y + 1 >= (Height + 7) / 8)
            {
                return;
            }
            memcpy(GFXGraphicsMemory + (CursorPosition.y + 1) * Width + CursorPosition.x, Hzk + data + 16, 16);
            CursorPosition.x += 16;
        }
        else
        {
            CursorPosition.y += 2;
            CursorPosition.x = 0;
        }
    }
    GFX_CheckPosition();
}
void GFX_Write(char *data, uint8_t length)
{
    while (length--)
    {
        GFX_WriteChar(*(data++));
    }
}
inline void GFX_Write(const char *data, uint8_t length)
{
    return GFX_Write((char *)data, length);
}
inline void GFX_Write(char *data)
{
    return GFX_Write(data, strlen(data));
}
inline void GFX_Write(const char *data)
{
    return GFX_Write(data, strlen(data));
}
void GFX_ClearRow(uint8_t Row)
{
    GFX_Changed = 1;
    memset(GFXGraphicsMemory + Row * Width, 0x00, Width);
}
void GFX_ClearRow_Ranged(uint8_t Row, uint8_t BeginCol, uint8_t StopCol)
{
    GFX_Changed = 1;
    memset(GFXGraphicsMemory + Row * Width + BeginCol,0x00, StopCol - BeginCol);
}
void GFX_InvertRow_Ranged(uint8_t Row, uint8_t BeginCol, uint8_t StopCol)
{
    GFX_Changed = 1;
    uint16_t i = StopCol - BeginCol;
    uint8_t *GFX_D = GFXGraphicsMemory + Row * Width + BeginCol;
    while (i--)
    {
        *GFX_D++ ^= 0xff;
    }
}
void GFX_InvertRow(uint8_t Row)
{
    GFX_Changed = 1;
    uint16_t i = Width;
    uint8_t *GFX_D = GFXGraphicsMemory + Row * Width;
    while (i--)
    {
        *GFX_D++ ^= 0xff;
    }
}
template <typename T_t>
inline void GFX_Write(T_t data)
{
    return GFX_Write(to_string(data));
}
template <typename T_t>
inline void GFX_WriteAt(T_t data, Point_t position)
{
    GFX_SetPosition(position.y, position.x);
    GFX_CheckPosition();
    return GFX_Write(to_string(data));
}
template <typename T_t>
inline void GFX_WriteAt(T_t data, uint8_t y, uint8_t x)
{
    GFX_SetPosition(y, x);
    GFX_CheckPosition();
    return GFX_Write(to_string(data));
}
Point_t TemplatePoint;
inline Point_t GFX_TextPosition(uint8_t y,uint8_t x){
    *(uint16_t*)&TemplatePoint = ((((uint16_t)x*6)<<8)|y);
    return TemplatePoint;
}
#endif
