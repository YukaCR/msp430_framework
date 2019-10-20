/*
*   Author: YukaCR
*   sukoi no tft font framework.
*	after call all time is under transfer.
*   also a great choice for other screens.
*   132us per comicsans number character at 45MHz speed.
*/
#pragma once
#include "fontlib/ComicSansNumber.h"
#include "fontlib/special.h"
#include "tft.h"
#include "cstring"

// 10 characters using DMA: 1.32ms seems great.
// 10 characters without DMA: 2.78ms. twice time with DMA, a little loss.
inline uint8_t fontlib_displayChar(const font_t& useFont, uint8_t index,uint8_t x, uint8_t y){
    // Fast address set.
    OLED_DC_Clr;
    OLED_CS_Clr;
    UCA0TXBUF = 0x2a;
    OLED_DC_Set;
    UCA0TXBUF = 0;
    UCA0TXBUF = x;
    UCA0TXBUF = 0;
    UCA0TXBUF = x + useFont.font_width - 1;
    OLED_DC_Clr;
    UCA0TXBUF = 0x2b;
    OLED_DC_Set;
    UCA0TXBUF = 0;
    UCA0TXBUF = y;
    UCA0TXBUF = 0;
    UCA0TXBUF = y + useFont.font_height - 1;
    OLED_DC_Clr;
    UCA0TXBUF = 0x2c;
    UCA0IFG = 0x00;
    OLED_DC_Set;
#if !defined(TFT_DO_NOT_USE_DMA_AFTER_INITILIZE)
    // Fast speed sending data;
    __data20_write_long((uintptr_t)&DMA1SA, (uintptr_t)(*(uint32_t*)((uint16_t)useFont.font_index + (index << 2))));//52mclk
    __data20_write_long((uintptr_t)&DMA1DA, (uintptr_t)&UCA0TXBUF);//14mclk
    DMA1SZ = useFont.font_size;
    DMA1CTL = DMAEN | DMASRCBYTE | DMADSTBYTE | DMASRCINCR_3;
	UCA0IFG |= UCTXIFG; 
	while(!(DMA1CTL & DMAIFG));
    OLED_CS_Set;
#else
    // Low speed sending data;
    register uint16_t position = (uint16_t)useFont.font_index[index];   // flash usage below 32kb this will work.
    register uint16_t target = useFont.font_size + position;
    while(position!=target){
        UCA0TXBUF = *(uint16_t*)(position ++);
    }
#endif
    return useFont.font_width;
}
inline uint8_t fontlib_displayChar(char str, uint8_t x, uint8_t y){
    if(str >= '0' && str <= '9'){
        return fontlib_displayChar(ComicSansNumber, str - 48, x, y);
    }
    else if(str == ','){
        return fontlib_displayChar(special, 0, x, y);
    }
    else if(str == '.'){
        return fontlib_displayChar(special, 1, x, y);
    }
    else if(str == '>'){
        return fontlib_displayChar(special, 2, x, y);
    }
}
inline uint8_t fontlib_displayString(char* str, uint8_t len,uint8_t x,uint8_t y){
    while(len--){
        x += fontlib_displayChar(*str++, x, y);
    }
}
inline uint8_t fontlib_displayString(char* str,uint8_t x,uint8_t y){
    return fontlib_displayString(str, strlen(str), x, y);
}
