/*
* Author: YukaCR
* 12bit dds buffer generation.
* can use up to 5kB ram. total ram of msp430f5529 is 8kb(without debug)/10kb.
*/
#include <msp430.h>
#include "stdlib.h"
#include "string.h"
#include "sin_table.h"
//volatile uint8_t DMA_SPI_Buffer[5001]; //7kb?????
union DMA_SPI_Buffer_Union {
    uint8_t DMA_SPI_Byte[5000];
    uint16_t DMA_SPI_Word[2500];
    uint32_t DMA_SPI_Long[1250];
    uint64_t DMA_SPI_Long_Long[625];
} DMA_SPI_Buffer_Dp;
uint16_t *DMA_SPI_Buffer = (uint16_t *)&DMA_SPI_Buffer_Dp;
#define DMA_Channel_Size_Reg DMA2SZ
const uint8_t ModuRation = 1;
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
        //freq = freq << 1;
        CurrentWave = SIN;
        if (ModuRation)
        {
            while (pos < (125000 * 4))
            {
                *(buffer += 2) = Sin(pos, ampl);
                pos += freq;
            }
        }
        else
        {
            while (pos < (125000 * 4))
            {
                result = Sin(pos, ampl) >> 2;
                *buffer++ = (result >> 8) & 0xff;
                *buffer++ = result & 0xff;
                pos += freq;
            }
        }
        buffer -= (uintptr_t)DMA_SPI_Buffer;
        DMA_Channel_Size_Reg = ((uint16_t)buffer) >> ModuRation;
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
        if (ModuRation)
        {
            while (i--)
            {
                result = (uint16_t)((float)i * pargma);
                *(buffer+=2) = result;
            }
        }
        else
        {
            while (i--)
            {
                result = (uint16_t)((float)i * pargma);
                result >>= 2;
                *buffer++ = result >> 8;
                *buffer++ = result & 0xff;
            }
        }
        buffer_int = buffer - 2;
        for (i = 0; i < c; i++)
        {
            *(uint16_t *)buffer = *(uint16_t *)buffer_int;
            buffer += 2;
            buffer_int -= 2;
        }

        buffer -= (uintptr_t)DMA_SPI_Buffer;
        DMA_Channel_Size_Reg = ((uint16_t)buffer) >> ModuRation;
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
        if (ModuRation)
        {
            while (i--)
            {
                *(buffer += 2) = pargma;
            }
        }
        else
        {
            pargma >>= 2;
            while (i--)
            {
                *buffer++ = pargma >> 8;
                *buffer++ = pargma & 0xff;
            }
        }
        memset(buffer, 0x00, c << 1);
        buffer -= 1;
        uint16_t data_len = (uintptr_t)buffer - (uintptr_t)DMA_SPI_Buffer + (c << 1) + 1;
        DMA_Channel_Size_Reg = data_len >> ModuRation;
    };
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
