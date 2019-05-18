/*
 * ad.c
 *
 *  Created on: 2014-7-16
 *      Author: Administrator
 */
#include "ad.h"
void ADInit(void)
{
	ADC12CTL0 |= ADC12MSC;									//�Զ�ѭ������ת��
	ADC12CTL0 |= ADC12ON;									//����ADC12ģ��
	ADC12CTL1 |= ADC12CONSEQ1 ;								//ѡ��ͨ��ѭ������ת��
	ADC12CTL1 |= ADC12SHP;									//��������ģʽ
	ADC12MCTL0 |= ADC12INCH_6; 								//ѡ��ͨ��5�����Ӳ����λ��
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;
}


