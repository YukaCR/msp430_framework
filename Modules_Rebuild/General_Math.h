#ifndef G_Math
#define G_Math

#define UseQSeries
/*
*   Calculate effective value
*/
//integral_0^((2 π)/ω) (C_0 sin(ω x) + C_0/2 + C_1) dx = (π (C_0 + 2 C_1))/ω
//result (Pi (Subscript[C, 0] + 2 Subscript[C, 1]))/ω
// C0: max-min , C1: min, C0 + 2C1=  max - min + 2 * min = max + min
float GM_Effective_Value(float max,float min,float freq){
    return (max + min) / (2 * freq);
}
#endif
