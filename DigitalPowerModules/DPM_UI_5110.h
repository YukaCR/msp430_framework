#ifndef DPM_UI_5110
#define DPM_UI_5110
#include "DPM.h"
#include "../Modules_Rebuild/Nokia5110.h"
#include "../Modules/ButtonMartix.h"
#define DPM_Input_Buffer_Size 20

//DPM UI
enum DPM_UI_InputStatus_t{
    DPM_UI_InputFree=0, DPM_UI_InputQueryed
}DPM_UI_InputStatus;

char DPM_Input_Buffer[DPM_Input_Buffer_Size];
char* DPM_Input_Position;
DPM_DATA_t DPM_Input_Result=0;

enum DPM_Selection_t{
    Current=0, Voltage, Freqency
}DPM_UI_Selection;
void DPM_Setup_UI();
void DPM_UI_Update(){
    GFX_ClearRow_Ranged(1,6,72);
    GFX_ClearRow_Ranged(3,6,78);
    GFX_ClearRow_Ranged(4,52,72);
    GFX_ClearRow(5);
    switch(DPM_UI_Selection){
        case Current:
            GFX_InvertRow_Ranged(1,6,72);
            break;
        case Voltage:
            GFX_InvertRow_Ranged(3,6,78);
            break;
        case Freqency:
            GFX_InvertRow_Ranged(4,52,72);
            break;
    }
    if(DPM_UI_InputStatus){
        GFX_WriteAt(DPM_Input_Buffer,5,0);  
    }
}
void DPM_UI_Input_Stop(){
    DPM_UI_Input_Result = DPM_DATA_In_Str_Convertion(DPM_Input_Buffer);
    DPM_Input_Position = DPM_Input_Buffer;
    memset(DPM_Input_Buffer,0x00,sizeof(DPM_Input_Buffer));
    DPM_UI_InputStatus = DPM_UI_InputFree;
}
void DPM_UI_Input_Srv(char KeyCode){
    if(!DPM_UI_InputStatus){
        DPM_UI_InputStatus = DPM_UI_InputQueryed;
    }
    if(KeyCode!='\n'){
        *(DPM_Input_Position++) = KeyCode;
    }
    else{
        DPM_UI_Input_Stop();
    }
    DPM_UI_Update();
}
#endif