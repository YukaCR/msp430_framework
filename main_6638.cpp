#include "vscode.h"
#include "Modules_Rebuild/DDS.h"
#include "DigitalPowerModules/DPM.h"
#include "TIDriver/dac12_a.h"
#include "TIDriver/timer_a.h"
#include "music.h"
void waitKey();
int main()
{
    // DAC
    P7DIR |= BIT6;
    P7SEL |= BIT6;
    DAC12_0CTL0 = DAC12IR + DAC12SREF_1 + DAC12AMP_5 + DAC12CALON + DAC12OPS;
    DAC12_0CTL0 = DAC12ENC;
    DAC12_0DAT = 4096 >> 1; // DAC12_0DAT * 3.3 / 4096
    // DMA
    DMACTL1 = 0;
    DMACTL1 |= DMA2TSEL__TA2CCR0;
    DMACTL4 = DMARMWDIS; 
    DMA2CTL = DMADT_5 + DMASRCINCR_3 + DMAEN;
    __data20_write_long((uintptr_t)&DMA2SA, (uintptr_t)&DMA_SPI_Buffer_Dp);
    __data20_write_long((uintptr_t)&DMA2DA, (uintptr_t)&DAC12_0DAT);
    // DDS Buffer
    ChangeSinProp(2000, 1.0f);
    // Timer
    UCS_initClockSignal(UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1); // 4MHz
    TA2CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA2CCR0 = 4.0f * 1000.0f * 1000.0f / 500.0f / 1000.0f;
    TA2CCTL0 = CM_1 | SCS | SCCI;

    TA1CTL = TASSEL__SMCLK | ID_0 | MC__STOP;
    TA1CCR0 = 4.0f * 1000.0f * 1000.0f / 1000.0f;
    TA1CCTL0 = CM_1 | SCS | SCCI;
    // exam 1
    waitKey();
    ChangeTriProp(2000, 1.0);   
    TA2CTL |= MC__UP | TACLR;   // start dds
    waitKey();
    ChangeSinProp(2000, 1.0);
    waitKey();
    TA2CTL |= MC__UP | TACLR;   // start dds
    uint16_t time = 1;
    uint16_t* times_ = (uint16_t*)times;
    uint16_t* tones_ = (uint16_t*)tones;
    while(1){   // play song!
        if(TA1CTL & TAIFG){
            time--;
            if(!time){
                ChangeSinProp(*tones_++, 1.0);
                time = *times_++;
                if(tones_ - tones == sizeof(tones) / sizeof(int)){
                    tones_ = (uint16_t*)tones;
                    times_ = (uint16_t*)times;
                }
            }
            TA1CTL &=~ TAIFG;
        }
    }
}