#include <msp430.h> 
#include "vscode.h"
#include "stdint.h"
#include "Modules/1602.h"
#include "Modules/ButtonMartix.h"
#include "profile_int.h"
void ButtonISR(uint8_t key){
    setCursor(0,1);
    printInt(key);
}
int main(){
    Button_Martix_Init();
    ButtonCallback = ButtonISR;
    __init__();
	init();
	home();
    printStr("wtmsb");
    while(1){continue;}
}
