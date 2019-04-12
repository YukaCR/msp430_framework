#ifndef Q15_H
#define Q15_H
#include "msp430.h"
#include "vscode.h"
float u8tofloat_trick(uint8_t x)
{
    union { float f; uint32_t i; } u; u.f = 32768.0f; u.i |= x;
    return u.f - 32768.0f;
}
float u8tofloat_trick2(uint8_t x)
{
    union { float f; uint32_t i; } u; u.f = 32768.0f; u.i |= x;
    return (u.f - 32768.0f)  / 255.0f;
}
float u16tofloat_trick(uint16_t x)
{
    union { float f; uint32_t i; } u; u.f = 16777216.0f; u.i |= x;
    return u.f - 16777216.0f; // optionally: (u.f - 16777216.0f) * (65536.0f / 65535.0f)
}
float u16tofloat_trick2(uint16_t x)
{
    union { float f; uint32_t i; } u; u.f = 16777216.0f; u.i |= x;
    return (u.f - 16777216.0f) / 65535.0f; // optionally: (u.f - 16777216.0f) * (65536.0f / 65535.0f)
}
static inline uint8_t u8fromfloat_trick(float x)
{
    union { float f; uint32_t i; } u;
    u.f = 32768.0f + x * (255.0f / 256.0f);
    return (uint8_t)u.i;
}
static inline uint16_t u16fromfloat_trick(float x)
{
    union { float f; uint32_t i; } u;
    u.f = 16777216.0f + x * (65536.0f / 65535.0f);
    return (uint8_t)u.i;
}
#endif
