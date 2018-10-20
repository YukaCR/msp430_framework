#include "i2c.h"
#include "oledfont.h"
#define OLED_ADDRESS 0x3c
const uint8_t oled_init_data[29]={0XAE,0X00,0X10,0X40,0X81,
		0XCF,0XA1,0XC8,0XA6,0XA8,0X3F,
		0XD3,0X00,0XD5,0X80,0XD9,0XF1,
		0XDA,0X12,0XDB,0X40,0X20,0X02,
		0X8D,0X14,0XA4,0XA6,0XAF,0XAF};
uint8_t current_font = 0;
uint8_t x = 0;
uint8_t	y = 0;
void oled_write(uint8_t data);
void OLED_setXY(uint8_t _x,uint8_t _y);
void oled_write(const uint8_t *buffer, size_t size);
void OLED_fill(uint8_t data);
void OLED_init();
int i = 0;
#pragma vector = RESET_VECTOR
interrupt void RESET_ISR(){
	i ++;
	P1OUT |= i;
	_nop();
	_nop();
}
int main(){
    OLED_init();
    while(1){
        continue;
    }
}
void oled_write(const uint8_t *buffer, size_t size){
	uint8_t i;
	for(i = 0;i<size;i++){
        oled_write(buffer[i]);
    }
}
void oled_write(uint8_t data){
    if(current_font!=2){
        if(data >= 32 && data <= (uint8_t)'z'){
            if(current_font){
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)F8X16[data - 32],8,5);
                OLED_setXY(x,y+1);
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(F8X16[data - 32] + 8),8,5);
                OLED_setXY(x+8,y-1);
            }
            else{
                i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)F6x8[data - 32],6,5);
                x=x+6;
            }
        }
        else if(data == '\n'){
            OLED_setXY(0,y+1+current_font);
        }
    }
    else{
        if(data!='\n'){
            i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(Hzk[data]),16,5);
            OLED_setXY(x,y+1);
            i2c_write_block(OLED_ADDRESS,0x40,(uint8_t*)(Hzk[data]+16),16,5);
            OLED_setXY(x>=112?x+16:0,y);
        }
        else{
            OLED_setXY(0,y>=6?0:y+2);
        }
    }
    return;
}
void OLED_setXY(uint8_t _x,uint8_t _y){
    uint8_t set_posion[3];
    set_posion[0] = 0xb0+_y;
    set_posion[1] = ((_x&0xf0)>>4)|0x10;
    set_posion[2] = (_x&0x0f)|0x01;
    i2c_write_block(OLED_ADDRESS,0x00,set_posion,3,5);
    x = _x;
    y = _y;
};
void OLED_fill(uint8_t data){
    uint8_t cmd[3]={0xb0,0x00,0x10};
	for(uint8_t i=0;i<8;i++)
	{
        i2c_write_block(OLED_ADDRESS,0x00,cmd,3,5);
		for(uint8_t n=0;n<128;n++)i2c_write_block(OLED_ADDRESS,0x40,data,5);
	}
};
void OLED_init(){
    i2c_master_init();
    i2c_write_block(OLED_ADDRESS,0x00,(uint8_t*)oled_init_data,29,5);
    OLED_setXY(0,0);
}
