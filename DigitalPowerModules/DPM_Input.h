#ifndef DPM_Input
#define DPM_Input
#include "DPM.h"
enum DPM_InputStatus_t{
    DPM_UI_InputFree=0, DPM_UI_InputQueryed
}DPM_InputStatus;

char DPM_Input_Buffer[DPM_Input_Buffer_Size];
char* DPM_Input_Position;
DPM_DATA_t DPM_Input_Result=0;
bool DPM_Input_DataChanged = 0;

enum DPM_Selection_t{
    Current=0, Voltage, Freqency
}DPM_Selection;

void DPM_Input_Srv(char);
void DPM_Input_Stop();
void DPM_Input_Init();

#endif