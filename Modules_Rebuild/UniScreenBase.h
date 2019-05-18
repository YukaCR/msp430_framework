#ifndef __USB_H__
#define __USB_H__
#include "stdint.h"
#include "string"
#include "cstring"
struct GraphicsMemory{
    uint8_t* DisplayMemory;
    uint8_t* CommandMemory;
    uint8_t* CommandQueryMemory;
    uint8_t NOP_Value;
};
struct ScreenInterface{
    GraphicsMemory Memory;
    uint16_t DisplayMemorySize;
    uint16_t CommandMemorySize;
    void (*WriteCommand)();
    void (*WriteDisplay)();
    void (*SetPosition)();
};
template <typename T>
void _usb_gfx_print(T Data,ScreenInterface Screen);
struct GraphicsInfomation{
    uint8_t width,height;
    uint8_t BackLight;
    uint8_t color_bits;//maybe rgb
};
template <typename T>
struct GFX_Operation{
    ScreenInterface Screen;
    void (*print)(T Data) = _usb_gfx_print(Data,Screen);
};
void _usb_gfx_print(char Data, ScreenInterface Screen){
    
}
void _usb_gfx_print(char* Data,ScreenInterface Screen){
    uint8_t n = strlen(Data);
    for(uint8_t i = 0; i <  n; i ++){
        
    }
}

#endif