#ifndef DAC7612U_H
#define DAC7612U_H
//max 10kHz, 500ksps, 50 dots.
//min 100Hz, 500ksps, 5000 dots.
//why not 1Hz, 250kSps, 250000dots, use data compress.
//500, 000 / 4 Bytes at rasp.

#include "DMA_SPI.h"
#include "sin_table.h"
#include "stdlib.h"
#include "string.h"
volatile uint8_t DMA_SPI_Buffer[5001]; //7kb?????
enum Waveform
{
    NONE,
    SIN,
    SQUARE,
    TRIANGLE
};
uint32_t freqency = 0;
float _ampl = 1.0;
Waveform CurrentWave = NONE;
void ChangeSinProp(uint32_t freq, float ampl)
{
    if (freq > 100000)
    {
        return;
    }
    else
    {
        uint8_t *buffer = (uint8_t *)DMA_SPI_Buffer;
        uint16_t result;
        uint32_t pos = 0;
        freqency = freq;
        _ampl = ampl;
        freq = freq << 1;
        CurrentWave = SIN;
        while (pos < (125000 * 4))
        {
            result = Sin(pos, ampl) >> 2;
            *buffer++ = (result >> 8) & 0xff;
            *buffer++ = result & 0xff;
            pos += freq;
        }
        buffer -= 1;
        DMA1SZ = buffer - DMA_SPI_Buffer + 1;
    };
}
void ChangeTriProp(uint32_t freq, float ampl)
{
    if (freq > 100000)
    {
        return;
    }
    else
    {
        uint8_t *buffer = (uint8_t *)DMA_SPI_Buffer, *buffer_int;
        uint16_t result;
        uint16_t c = 125000 / freq, i;
        float pargma = (float)4096 * ampl / (float)c;
        CurrentWave = TRIANGLE;
        freqency = freq;
        _ampl = ampl;
        i = c;
        while (i--)
        {
            result = (uint16_t)((float)i * pargma);
            result >>= 2;
            *buffer++ = result >> 8;
            *buffer++ = result & 0xff;
        }
        buffer_int = buffer - 2;
        for (i = 0; i < c; i++)
        {
            *(uint16_t *)buffer = *(uint16_t *)buffer_int;
            buffer += 2;
            buffer_int -= 2;
        }
        buffer -= 1;
        DMA1SZ = buffer - DMA_SPI_Buffer + 1;
    };
}
void ChangeSqProp(uint32_t freq, float ampl)
{
    if (freq > 100000)
    {
        return;
    }
    else
    {
        uint8_t *buffer = (uint8_t *)DMA_SPI_Buffer;
        uint16_t c = 125000 / freq, i;
        uint16_t pargma = ((float)4095 * ampl);
        CurrentWave = SQUARE;
        freqency = freq;
        _ampl = ampl;
        i = c;
        pargma >>= 2;
        while (i--)
        {
            *buffer++ = pargma >> 8;
            *buffer++ = pargma & 0xff;
        }
        memset(buffer, 0x00, c << 1);
        buffer -= 1;
        DMA1SZ = buffer - DMA_SPI_Buffer + (c << 1) + 1;
    };
}
void Setup_DAC7612U_DDS()
{
    P6SEL &= ~BIT5;
    P6DIR |= BIT5;
    P6OUT |= BIT5;
    setup_DMA_SPI();
    ChangeSinProp(1000, 1);
}
void changeWaveForm(Waveform wave, uint32_t freq = 0, float ampl = 1.0)
{
    if (freq == 0)
    {
        freq = freqency;
        ampl = _ampl;
    }
    switch (wave)
    {
    case SIN:
        return ChangeSinProp(freq, ampl);
        break;
    case SQUARE:
        return ChangeSqProp(freq, ampl);
        break;
    case TRIANGLE:
        return ChangeTriProp(freq, ampl);
        break;
    case NONE:
        break;
    default:
        break;
    }
}
#if 0
void DAC_Write(uint16_t value,uint16_t head){
    unsigned char send_data_0,send_data_1;
    value|=(head<<12);
    send_data_0 = (value>>8)&0x00ff;
    send_data_1 = value&0x00ff;
    LOADDACS = 0;
    LOADDACS = 1;
    SPI_Write(send_data_0);
    SPI_Write(send_data_1);
    LOADDACS = 0;
    LOADDACS = 1;
}
#endif

#endif
