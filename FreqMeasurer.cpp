/* Developed ok.
#include <msp430.h>
#include "Freq.h"
#include "oled.h"
#include "stdlib.h"
#include "math.h"
void Freq_measurer_process(double data){
    char* number,*dot;
    data=data<0?-data:data;
    if(data > 10000){
        number = (char*)malloc(sizeof(char)*5);
        dot = (char*)malloc(sizeof(char)*5);
        data = data / (double)1000;
        itoa(floor(data),number,10);
        data -= floor(data);
        itoa(floor(data*10000),dot,10);
        OLED_fill(0x00);
        OLED_setXY(0,0);
        oled_write(number,strlen(dot));
        oled_write('.');
        if(data<0.1){
        	oled_write('0');
        }
        oled_write(dot,strlen(dot));
        oled_write("KHz",3);
        if(strlen(number) == 1&&strlen(dot) == 1&&number[0] == '2'&&dot[0] == '2'){
        	WDTCTL = 0xdead;//heap memory use up
        }
    }
    else if(data > 0){
        number = (char*)malloc(sizeof(char)*5);
        dot = (char*)malloc(sizeof(char)*5);
        itoa(floor(data),number,10);
        data -= floor(data);
        itoa(floor(data*10000),dot,10);
        OLED_fill(0x00);
        OLED_setXY(0,0);
        oled_write(number,strlen(dot));
        oled_write('.');
        if(data<0.1){
        	oled_write('0');
        }
        oled_write(dot,strlen(dot));
        oled_write("Hz",2);
    }
    else if(data == 0){
    	number = (char*)malloc(sizeof(char)*5);
    	dot = (char*)malloc(sizeof(char)*5);
        OLED_fill(0x00);
        OLED_setXY(0,0);
        oled_write("No Sauce.",9);
    }
    if(number==0x00||dot==0x00){
    	WDTCTL = 0xdead;//heap memory use up
    }
    free(number);
    free(dot);
    _freq_start();
}
*/
/*
int main(){
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    OLED_init();
    _freq_init();
    _freq_SetCallback(process);
    _freq_start();
    //UCS_initClockSignal(UCS_SMCLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    while(1){
        continue;
    }
}
*/