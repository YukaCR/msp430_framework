#include "DPM_Display.h"
#include "Modules/tft.h"
void DPM_Display_Init(){
    P4DIR |= BIT2 | BIT1;
    P4REN |= BIT2 | BIT1;
    P3DIR |= BIT5 | BIT2;
    P3REN |= BIT5 | BIT2;
    Setup_SPI_Master();
    Lcd_Init();
}
void DPM_Display_Update(){

}
void DPM_Display_Redraw(){
    
}