#include "msp430.h"
#include "vscode.h"
#include "Modules_Rebuild/DAC7612U.h"
#include "Modules/ButtonMartix.h"
#include "Modules_Rebuild/SSD1306.h"
#include "stdlib.h"
bool input_status = 0;
uint32_t input_buffer = 0;
uint8_t current_position = 0;
bool Current_Selected_isAmpl = 0;
bool dot_enabled = 0;
uint8_t dot_position = 0;
const char *kHz_Data[] = {" Hz", " kHz"};
int main()
{
    Setup_DAC7612U_DDS();
    Setup_SSD1306();
    Setup_Button_Martix();
    SSD1306_Clear();
    GFX_WriteAt("A:Sin B:Squ C:Tri\n", 0, 0);
    GFX_Write("D:Enter #:. *:^Cancel\n");
    GFX_Write("Freq: 1.0 kHz\n");
    GFX_Write("Ampl: 16.384 V\n");
    GFX_InvertRow_Ranged(0, 2 * 6, 5 * 6);
    GFX_InvertRow_Ranged(2, 6 * 6, 127);
    while (1)
    {
        if (Button_Flag)
        {
            switch (KeyCode)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (!input_status)
                {
                    input_status = 1;
                    GFX_SetPosition(7, 0);
                }
                input_buffer = input_buffer * 10 + KeyCode - 48;
                current_position++;
                GFX_Write(KeyCode);
                break;
            case '*':
                if (input_status)
                {
                    input_status = 0;
                    input_buffer = 0;
                    current_position = 0;
                    dot_enabled = 0;
                    dot_position = 0;
                    GFX_ClearRow(7);
                }
                else
                {
                    Current_Selected_isAmpl ^= 1;
                    GFX_InvertRow_Ranged(2, 6 * 6, 127);
                    GFX_InvertRow_Ranged(3, 6 * 6, 127);
                };
                break;
            case '#':
                if(Current_Selected_isAmpl && input_status && !dot_enabled){
                    dot_enabled = true;
                    dot_position = current_position;
                    GFX_Write('.');
                }
                break;
            case 'A':
                changeWaveForm(SIN);
                GFX_ClearRow(0);
                GFX_WriteAt("A:Sin B:Squ C:Tri\n", 0, 0);
                GFX_InvertRow_Ranged(0, 2 * 6, 5 * 6);
                break;
            case 'B':
                changeWaveForm(SQUARE);
                GFX_ClearRow(0);
                GFX_WriteAt("A:Sin B:Squ C:Tri\n", 0, 0);
                GFX_InvertRow_Ranged(0, 8 * 6, 11 * 6);
                break;
            case 'C':
                changeWaveForm(TRIANGLE);
                GFX_ClearRow(0);
                GFX_WriteAt("A:Sin B:Squ C:Tri\n", 0, 0);
                GFX_InvertRow_Ranged(0, 14 * 6, 17 * 6);
                break;
            case 'D':
                if (Current_Selected_isAmpl)
                {
                    float input_result = 0;
                    if (dot_position)
                    {
                        while (dot_position--)
                        {
                            input_result += input_buffer % 10;
                            input_result /= 10;
                            input_buffer /= 10;
                        }
                        input_result += input_buffer;
                    }
                    else
                    {
                        input_result = input_buffer;
                    }
                    input_result *= 0.06103515625;
                    if (input_result > 1 || input_buffer == 0)
                    {
                        break; //alert?
                    }
                    changeWaveForm(CurrentWave, freqency, input_result);
                    GFX_ClearRow(3);
                    GFX_WriteAt("Ampl: ", 3, 0);
                    GFX_Write(input_result * 16.384);
                    GFX_Write(" V");
                    GFX_InvertRow_Ranged(3, 6 * 6, 127);
                }
                else
                {
                    if(input_buffer){
                        changeWaveForm(CurrentWave, input_buffer, _ampl);
                        bool kHz = false;
                        float input_result = input_buffer;
                        if (input_result >= 1000)
                        {
                            input_result /= 1000;
                            kHz = 1;
                        };
                        GFX_ClearRow(2);
                        GFX_WriteAt("Freq: ", 2, 0);
                        GFX_Write(input_result);
                        GFX_Write(kHz_Data[kHz]);
                        GFX_InvertRow_Ranged(2, 6 * 6, 127);
                    }
                }
                input_status = 0;
                input_buffer = 0;
                current_position = 0;
                dot_enabled = 0;
                dot_position = 0;
                GFX_ClearRow(7);
                break;
            }
            Button_Flag = 0;
        }
    }
}
