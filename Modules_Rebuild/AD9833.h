// AD9833 function generator. translate from arduino.
#ifndef AD9833_H
/** \name AD9833 Control Register bit definitions
 * @{
 */
#include "stdint.h"
#include "GPIO.h"
#include "SPI.h"
#include "math.h"
#define AD_B28 13            ///< B28  1 allows a complete word to be loaded into a frequency register in \
                             ///< two consecutive writes. When B28  0, the 28-bit frequency register      \
                             ///< operates as two 14-bit registers.
#define AD_HLB 12            ///< Control bit allows the user to continuously load the MSBs or LSBs of a \
                             ///< frequency register while ignoring the remaining 14 bits. HLB is used   \
                             ///< in conjunction with B28; when B28  1, this control bit is ignored.
#define AD_FSELECT 11        ///< Defines whether the FREQ0 register or the FREQ1 register is used in \
                             ///< the phase accumulator.
#define AD_PSELECT 10        ///< Defines whether the PHASE0 register or the PHASE1 register data is \
                             ///< added to the output of the phase accumulator.
#define AD_RESET 8           ///< Reset  1 resets internal registers to 0, which corresponds to an \
                             ///< analog output of midscale. Reset  0 disables reset.
#define AD_SLEEP1 7          ///< When SLEEP1  1, the internal MCLK clock is disabled, and the DAC output \
                             ///< remains at its present value. When SLEEP1  0, MCLK is enabled.
#define AD_SLEEP12 6         ///< SLEEP12  1 powers down the on-chip DAC. SLEEP12  0 implies that \
                             ///< the DAC is active.
#define AD_OPBITEN 5         ///< When OPBITEN  1, the output of the DAC is no longer available at the  \
                             ///< VOUT pin, replaced by MSB (or MSB/2) of the DAC. When OPBITEN  0, the \
                             ///< DAC is connected to VOUT.
#define AD_DIV2 3            ///< When DIV2  1, the MSB of the DAC data is passed to the VOUT pin. When \
                             ///< DIV2  0, the MSB/2 of the DAC data is output at the VOUT pin.
#define AD_MODE 1            ///< When MODE  1, the SIN ROM is bypassed, resulting in a triangle output \
                             ///< from the DAC. When MODE  0, the SIN ROM is used which results in a    \
                             ///< sinusoidal signal at the output.
#define AD_DEFAULT_FREQ 1000 ///< Default initialisation frequency (Hz)
#define AD_DEFAULT_PHASE 0   ///< Default initialisation phase angle (degrees)
#define AD_FREQ1 15          ///< Select frequency 1 register
#define AD_FREQ0 14          ///< Select frequency 0 register
#define AD_PHASE 13          ///< Select the phase register
#define SEL_FREQ0 (1 << AD_FREQ0)
#define SEL_FREQ1 (1 << AD_FREQ1)
#define SEL_PHASE0 (1 << AD_FREQ0 | 1 << AD_FREQ1 | 0 << AD_PHASE)
#define SEL_PHASE1 (1 << AD_FREQ0 | 1 << AD_FREQ1 | 1 << AD_PHASE)
#define AD_MCLK 25000000UL    ///< Clock speed of the AD9833 reference clock in Hz
#define AD_2POW28 (1UL << 28) ///< Used when calculating output frequency

/** Macro to calculate register value for AD9833 frequency register from a frequency */
#define FREQ_CALC(f) (float)(round(f * (double)AD_2POW28 / (double)AD_MCLK))

/** Macro to calculate the value for AD9833 phase register from given phase in tenths of a degree */
#define CALC_PHASE(a) (uint16_t)(512.0 * ((float)a / 10.0) / 45.0)
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

enum channel_t
{
    CHAN_0 = 0, ///< Channel 0 definition
    CHAN_1 = 1, ///< Channel 1 definition
};
enum mode_t
{
    MODE_OFF,      ///< Set output all off
    MODE_SINE,     ///< Set output to a sine wave at selected frequency
    MODE_SQUARE1,  ///< Set output to a square wave at selected frequency
    MODE_SQUARE2,  ///< Set output to a square wave at half selected frequency
    MODE_TRIANGLE, ///< Set output to a triangle wave at selected frequency
};
struct AD9833_Memory
{
    uint16_t _regCtl;      // control register image
    uint32_t _regFreq[2];  // frequency registers
    uint32_t _regPhase[2]; // phase registers

    // Settings memory
    mode_t _modeLast;   // last set mode
    float _freq[2];     // last frequencies set
    uint16_t _phase[2]; // last phase setting

    //pinout reg
    uint8_t fsyncPinMask;
    const GPIO_Struct *fsyncPinPort;
} AD9833;
bool AD9833_SetFrequency(channel_t chan, float freq)
{
    uint16_t freq_select;

    AD9833._freq[chan] = freq;

    AD9833._regFreq[chan] = FREQ_CALC(freq);

    // select the address mask
    switch (chan)
    {
    case CHAN_0:
        freq_select = SEL_FREQ0;
        break;
    case CHAN_1:
        freq_select = SEL_FREQ1;
        break;
    }

    // Assumes B28 is on so we can send consecutive words
    // B28 is set by default for the library
    // Now send the two parts of the frequency 14 bits at a time,
    // LSBs first
    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
    spi_send(freq_select | (uint16_t)((uint32_t)AD9833._regFreq[chan] & 0x3fff));
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
    spi_send(freq_select | (uint16_t)((uint32_t)AD9833._regFreq[chan] >> 14) & 0x3fff);
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
    return (true);
}
bool AD9833_SetPhase(channel_t chan, uint16_t phase)
{
  uint16_t  phase_select;

  AD9833._phase[chan] = phase;
  AD9833._regPhase[chan] = CALC_PHASE(phase);

  // select the address mask
  switch (chan)
  {
  case CHAN_0:  phase_select = SEL_PHASE0; break;
  case CHAN_1:  phase_select = SEL_PHASE1; break;
  }

  // Now send the phase as 12 bits with appropriate address bits
    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
  spi_send(phase_select | (0xfff & AD9833._regPhase[chan]));
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
  
  return(true);

}
bool AD9833_SetMode(mode_t mode)
{
  AD9833._modeLast = mode;

  switch (mode)
  {
  case MODE_OFF:
    bitSet(AD9833._regCtl, AD_SLEEP1);
    bitSet(AD9833._regCtl, AD_SLEEP12);
    break;
  case MODE_SINE:
    bitClear(AD9833._regCtl, AD_OPBITEN);
    bitClear(AD9833._regCtl, AD_MODE);
    bitClear(AD9833._regCtl, AD_SLEEP1);
    bitClear(AD9833._regCtl, AD_SLEEP12);
    break;
  case MODE_SQUARE1:
    bitSet(AD9833._regCtl, AD_OPBITEN);
    bitClear(AD9833._regCtl, AD_MODE);
    bitSet(AD9833._regCtl, AD_DIV2);
    bitClear(AD9833._regCtl, AD_SLEEP1);
    bitClear(AD9833._regCtl, AD_SLEEP12);
    break;
  case MODE_SQUARE2:
    bitSet(AD9833._regCtl, AD_OPBITEN);
    bitClear(AD9833._regCtl, AD_MODE);
    bitClear(AD9833._regCtl, AD_DIV2);
    bitClear(AD9833._regCtl, AD_SLEEP1);
    bitClear(AD9833._regCtl, AD_SLEEP12);
    break;
  case MODE_TRIANGLE:
    bitClear(AD9833._regCtl, AD_OPBITEN);
    bitSet(AD9833._regCtl, AD_MODE);
    bitClear(AD9833._regCtl, AD_SLEEP1);
    bitClear(AD9833._regCtl, AD_SLEEP12);
    break;
  }

    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
  spi_send(AD9833._regCtl);

    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
  return(true);
}
channel_t AD9833_GetActivePhase(void)
{ 
  return bitRead(AD9833._regCtl, AD_PSELECT) ? CHAN_1 : CHAN_0; 
};
bool AD9833_SetActivePhase(channel_t chan)
{

  switch (chan)
  {
  case CHAN_0: bitClear(AD9833._regCtl, AD_PSELECT); break;
  case CHAN_1: bitSet(AD9833._regCtl, AD_PSELECT);   break;
  }

    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
  spi_send(AD9833._regCtl);

    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
  return(true);
}
channel_t AD9833_GetActiveFrequency(void)
{ 
  return bitRead(AD9833._regCtl, AD_FSELECT) ? CHAN_1 : CHAN_0; 
};
bool AD9833_SetActiveFrequency(channel_t chan)
{

  switch (chan)
  {
  case CHAN_0: bitClear(AD9833._regCtl, AD_FSELECT); break;
  case CHAN_1: bitSet(AD9833._regCtl, AD_FSELECT);   break;
  }

    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
  spi_send(AD9833._regCtl);
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;

  return(true);
}
void AD9833_Setup(const GPIO_Struct *fsyncPinPort, uint8_t fsyncPinMask)
{  
    P2DIR |= BIT2;                            // SMCLK set out to pins
    P2SEL |= BIT2;                            
    AD9833.fsyncPinPort = fsyncPinPort;
    AD9833.fsyncPinMask = fsyncPinMask;
    SPI_Setup();
    HWREG8(fsyncPinPort->DR) |= fsyncPinMask;
    HWREG8(fsyncPinPort->ODR) |= fsyncPinMask;
    AD9833._regCtl = 0;
    bitSet(AD9833._regCtl, AD_B28);   // always write 2 words consecutively
    bitSet(AD9833._regCtl, AD_RESET); // Reset is done on a 1 to 0 transition
    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
    spi_send(AD9833._regCtl);
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;
    
    AD9833_SetFrequency(CHAN_0, AD_DEFAULT_FREQ);
    AD9833_SetFrequency(CHAN_1, AD_DEFAULT_FREQ);
    AD9833_SetPhase(CHAN_0, AD_DEFAULT_PHASE);
    AD9833_SetPhase(CHAN_1, AD_DEFAULT_PHASE);
    bitClear(AD9833._regCtl, AD_RESET);
    HWREG8(AD9833.fsyncPinPort->ODR) &=~ AD9833.fsyncPinMask;
    spi_send(AD9833._regCtl);
    HWREG8(AD9833.fsyncPinPort->ODR) |= AD9833.fsyncPinMask;

    AD9833_SetMode(MODE_TRIANGLE);
    AD9833_SetActiveFrequency(CHAN_0);
    AD9833_SetActivePhase(CHAN_0);
}
#endif