#include "DPM_Display.h"
#include "Modules/tft.h"
#include "Modules/tft_font.h"
#define FontColor WHITE
#define BackColor BLACK

void DPM_Display_Init(){
    P4DIR |= BIT2 | BIT1;
    P4REN |= BIT2 | BIT1;
    P3DIR |= BIT5 | BIT2;
    P3REN |= BIT5 | BIT2;
    Lcd_Init();
    LCD_Clear(BackColor);
}
void DPM_Display_Update(){

}
void DPM_Display_Redraw(){
    
}
