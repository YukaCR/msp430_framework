#include <msp430.h>
#define Mhz 1000000L
#define Khz 1000L
#include "TIDriver/ucs.h"
#include "PWM.h"
#include "vscode.h"
int main()
{
    InitPWMPercent(50,10);

  P1DIR |= BIT0;                            // ACLK set out to pins
  P1SEL |= BIT0;                            
  P2DIR |= BIT2;                            // SMCLK set out to pins
  P2SEL |= BIT2;                            
  P7DIR |= BIT7;                            // MCLK set out to pins
  P7SEL |= BIT7;                                                  
    while (1)
    {
        continue;
    }
}
