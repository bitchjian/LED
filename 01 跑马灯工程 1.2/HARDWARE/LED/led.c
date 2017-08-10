#include "led.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//����������������
//���ߣ�����
//ʱ�䣺2015.10.10
//qq��750209408
//////////////////////////////////////////////////////////////////////////////////////////////////

//LED���ų�ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE); //ʹ��PB��PE�˿�ʱ��
	//PB.5�˿ڲ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                 //PB5�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                          //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         //IO���ٶ�Ϊ50MHZ
	GPIO_Init(GPIOB,&GPIO_InitStructure);                                     //��ʼ��PB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0);                                           //����PB.5�����
	//PE.5�˿ڲ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                 //PB5�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                          //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         //IO���ٶ�Ϊ50MHZ
	GPIO_Init(GPIOE,&GPIO_InitStructure);                                     //��ʼ��PB.5
	GPIO_SetBits(GPIOE,GPIO_Pin_5);                                           //����PB.5�����
}

