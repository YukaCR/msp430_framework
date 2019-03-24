#ifdef __VS_CODE__
#ifndef __VS_CODE_H__
#include <stdint-gcc.h>
#include <msp430.h>
#define interrupt
#endif // !__VS_CODE_H__
int main(){
    WDTCTL = WDTPW | WDTHOLD;
    _main();
}
#define main() inline _main()
#endif // __VS_CODE__
