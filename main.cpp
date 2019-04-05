#include <msp430.h> 
#include "vscode.h"
#include "stdint.h"
#include "Modules/Freq+.h"
#include "Modules/1602.h"
#include "Modules/ButtonMartix.h"
#include "profile_int.h"
inline void ButtonISR(uint8_t key){
    setCursor(0,1);
    printStr("      ");
    setCursor(0,1);
    printInt(key);
}
inline void FreqISR(double data){
    uint32_t _data = 0;
    _data += ceil(data);
    setCursor(0,0);
    printStr("              ");
    setCursor(0,0);
    printStr("zH");
    while(_data>0){
        printChar(_data%10 + 48);
        _data /= 10;
    }
}
int main(){
    __init__();
    init();
    clear();
    home();
    FreqInit();
    FreqStart();
    setCursor(0,1);
    printInt(10000);
    while(1){
        if(Freq_Flag){
            FreqISR(FreqResult);
            Freq_Flag = 0;
            FreqStart();
        }
    }
}
