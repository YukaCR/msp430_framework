#ifndef ADS8588_H
#define ADS8588_H
#include "../IQMath/QmathLib.h"
#include "msp430.h"
#include "../vscode.h"
#include "SPI.h"
#ifndef ADS8588_MAX_CHANNEL
#define ADS8588_MAX_CHANNEL 8
#endif
_q15 Result[ADS8588_MAX_CHANNEL];

#define ADS8588_CS 
#define ADS8588_CONVST
#define ADS8588_RESET
#define ADS8588_BUSY
#define ADS8588_FRSTDATA

#define BIS_ADS8588_CS 
#define BIS_ADS8588_CONVST
#define BIS_ADS8588_RESET
#define BIS_ADS8588_BUSY
#define BIS_ADS8588_FRSTDATA

#define BIC_ADS8588_CS 
#define BIC_ADS8588_CONVST
#define BIC_ADS8588_RESET
#define BIC_ADS8588_BUSY
#define BIC_ADS8588_FRSTDATA

#define RR_ADS8588_CS 
#define RR_ADS8588_CONVST
#define RR_ADS8588_RESET
#define RR_ADS8588_BUSY 1
#define RR_ADS8588_FRSTDATA

void setup_ADS8588(){
    BIC_ADS8588_RESET;
    
    BIS_ADS8588_RESET;

}

void ADS8588_Read(){

    while(RR_ADS8588_BUSY);
    uint8_t i = 0;
    for(;i<ADS8588_MAX_CHANNEL;i++){
        Result[i] = spi_send((uint16_t)0xff);
    }
}

inline void ADS8588_StartConv(){
    BIC_ADS8588_CONVST;
    __delay_cycles(50);
    BIS_ADS8588_CONVST;
    BIS_ADS8588_CS;
}
#endif