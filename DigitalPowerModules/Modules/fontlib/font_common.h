/*
*   Author: YukaCR
*   sukoi no font framework has fast query speed and low memory usage.
*/
#pragma once
#include "stdint.h"
const struct font_t{
    const uint8_t font_height;
    const uint8_t font_width;
    const uint16_t font_size;
    const uintptr_t font;
    const uint16_t** font_index;
}; 
