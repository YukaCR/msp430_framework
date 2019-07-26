#ifndef DPM_Measure
#define DPM_Measure

#include "DPM.h"

void DPM_Current_Detect();   
void DPM_Voltage_Detect();
void DPM_Power_Detect(); 
void DPM_Setup_Voltage_Measure();
void DPM_Setup_Current_Measure();
void DPM_Setup_Power_Measure();
DPM_DATA_t DPM_Current;
DPM_DATA_t DPM_Voltage;
DPM_DATA_t DPM_Power;

bool DPM_Measure_End = false;

#endif