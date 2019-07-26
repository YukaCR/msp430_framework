#ifndef DPM_Feedback
#define PWM_Feedback
#include "DPM.h"
#include "DPM_Feedback.h"
#include "../IQMath/QmathLib.h"

#ifndef DPM_FB_PID_Coff_Dif
#define DPM_FB_PID_Coff_Dif 0.2
#endif

#ifndef DPM_FB_PID_Coff_Int
#define DPM_FB_PID_Coff_Int 0.015
#endif

#ifndef DPM_FB_PID_Coff_Prop
#define DPM_FB_PID_Coff_Prop 0.2
#endif

#if DPM_Output_Mode == DPM_Output_Mode_Current
    #define DPM_Target_Point DPM_Current_Sense_Coff * DPM_Output_Current / DPM_ADC_Reference_Voltage
#elif DPM_Output_Mode == DPM_Output_Mode_Voltage
    #define DPM_Target_Point DPM_Voltage_Sense_Coff * DPM_Output_Voltage / DPM_ADC_Reference_Voltage
#elif DPM_Output_Mode == DPM_Output_Mode_Power
    #define DPM_Target_Point DPM_Output_Power * DPM_Current_Sense_Coff * DPM_Voltage_Sense_Coff / DPM_ADC_Reference_Voltage
#else
    #warning "[DPM_Feedback_PID.h:25]"
    #define DPM_Target_Point 0
#endif

struct DPM_FB_PID_Coff_t
{
    DPM_DATA_t SetPoint;
    DPM_DATA_t FeedBack;
    DPM_DATA_t err;
    DPM_DATA_t err_last;
    DPM_DATA_t Kp, Ki, Kd;
    DPM_DATA_t Result;
    DPM_DATA_t integral;
} DPM_FB_PID_Coff = {DPM_Float_Convertion(DPM_Target_Point), DPM_Float_Convertion(0.0), DPM_Float_Convertion(0.0), DPM_Float_Convertion(0.0),
                     DPM_Float_Convertion(DPM_FB_PID_Coff_Prop), DPM_Float_Convertion(DPM_FB_PID_Coff_Int), DPM_Float_Convertion(DPM_FB_PID_Coff_Dif), DPM_Float_Convertion(0.0),
                     DPM_Float_Convertion(0.0)};

uintptr_t DPM_FB_ADC_Sense = (uintptr_t)&(DPM_FB_PID_Coff.FeedBack);
uintptr_t DPM_FB_Result_Reg = (uintptr_t)&(DPM_FB_PID_Coff.Result);

void DPM_Feedback_Calculation()
{
    DPM_FB_PID_Coff.err = DPM_FB_PID_Coff.SetPoint - DPM_FB_PID_Coff.FeedBack;
    DPM_FB_PID_Coff.integral += DPM_FB_PID_Coff.err;
    DPM_FB_PID_Coff.Result = DPM_FB_PID_Coff.Kp * DPM_FB_PID_Coff.err + DPM_FB_PID_Coff.Ki * DPM_FB_PID_Coff.integral + DPM_FB_PID_Coff.Kd * (DPM_FB_PID_Coff.err - DPM_FB_PID_Coff.err_last); //
    DPM_FB_PID_Coff.err_last = DPM_FB_PID_Coff.err;
}
#endif