#include "../DigitalPowerModules/DPM_PWMControl_92MHz_40kHz.h"
#include "../vscode.h"
void Setup_Motor(){
    DPM_PWMC_Setup_PWMControl();
    TA2CCR2 = 2048;
    TA2CCR1 = 2048;
}
void TurnLeft(){
    TA2CCR1 = 2048;
    TA2CCR2 = 1600;
}
void TurnRight(){
    TA2CCR1 = 1600;
    TA2CCR2 = 2048;
}
void NormalRun(){
    TA2CCR1 = TA2CCR2 = 2048;
}
