/*
*   Digital power module, framework header.
*   declare all of the functions and variables in this header.
*/
#ifndef DPM
#define DPM
#pragma once
#include "DPM_Constant.h"
// DPM configure
#define DPM_Input_Buffer_Size 20

#define DPM_DATA_FORMAT             _q12            // Digital Power Module Type
#define DPM_DATA_Out_Str_Convertion _Q12toa
#define DPM_DATA_In_Str_Convertion  _atoQ12
#define DPM_Float_Convertion        _Q12
#define GLOBAL_Q                12
#define GLOBAL_IQ               24

#define DPM_ADC_Reference_Voltage 3.27

#define DPM_Input_Voltage  16.0
#define DPM_Output_Voltage 12.0
#define DPM_Output_Current 0.0
#define DPM_Output_Power   0.0
#define DPM_Output_Mode    DPM_Output_Mode_Voltage
#define DPM_Voltage_Sense_Coff 0.1
#define DPM_Current_Sense_Coff 0.1


// system headers 
#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// useful functions
#include "../vscode.h"

// accelerate calculation 
#include "../IQMath/IQmathLib.h"
#include "../IQMath/QmathLib.h"

// modules
#include "../Modules/MPY32.h"               // for accelerate calculation

// DPM system Modules
/* List of DPM Modules
*   DPM_Display             Display some important information such as current, voltage and power
*   DPM_Feedback            Use different feedback to originize defferent calcuation cycles
*   DPM_Input               User Input
*   DPM_Measure             Measure important information such as current, voltage and power
*   DPM_MeasureProcesser    Select which sense module you prefered
*   DPM_Method              Select which main power circuit you prefered
*   DPM_PWMControl          Control the pwm output
*/
#include "DPM_Display_TFT.h"
/* 
#include "DPM_Measure_BuiltinADC.h"
#include "DPM_PWMControl_33MHz_33kHz.h"
#include "DPM_Display_TFT.h"
#include "DPM_Input_KeyPad.h"
#include "DPM_Feedback_PID.h"
#include "DPM_Method_Buck.h"
*/

typedef DPM_DATA_FORMAT DPM_DATA_t;

/* DPM module setup functions */
#ifdef DPM_Measure
extern void DPM_Setup_Measure();
#endif

// DPM Display
#ifdef DPM_Display
extern void DPM_Display_Init();
extern void DPM_Display_Update();
extern void DPM_Display_Redraw();
#endif 

// DPM Input variables and functions
#ifdef DPM_Input
extern void DPM_Input_Srv(char);
extern void DPM_Input_Stop();
extern void DPM_Input_Init();
extern DPM_InputStatus_t DPM_UI_InputStatus;
extern DPM_Selection_t DPM_Selection;
extern char DPM_Input_Buffer[];
extern char* DPM_Input_Position;
extern bool DPM_Input_DataChanged;
#endif

// method for current sense, input, dpm_measure
#ifdef DPM_Measure
extern void DPM_Current_Detect();   
extern void DPM_Voltage_Detect();
extern void DPM_Power_Detect(); 
extern void DPM_Method_Init();
extern DPM_DATA_t DPM_Current;
extern DPM_DATA_t DPM_Voltage;
extern DPM_DATA_t DPM_Power;   
extern bool DPM_Measure_End;
#endif

// method for PWM Control
#ifdef DPM_PWMControll
extern void DPM_PWMC_Change_Freqency(DPM_DATA_t);
extern void DPM_PWMC_Change_DutyCycle(uint16_t);
extern DPM_DATA_t DPM_Current_Freqency;
#endif

// method for feedback, data input, output
#ifdef DPM_Feedback
extern void DPM_Feedback_Calculation();
extern uintptr_t DPM_FB_ADC_Sense;
extern uintptr_t DPM_FB_Result_Reg;
#endif

// method for digital power control method, input, output
#ifdef DPM_Method
extern uint16_t DPM_Data_to_PWM_Value(DPM_DATA_t);
#endif

void DPM_Service_LOOP();

void Setup_DPM();

int dpm_main(){
    
    while(1){
        
    }
    return 0;
}

#endif
