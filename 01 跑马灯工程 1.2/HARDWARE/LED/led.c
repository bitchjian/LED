#include "led.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//跑马灯驱动程序代码
//作者：辛雨
//时间：2015.10.10
//qq：750209408
//////////////////////////////////////////////////////////////////////////////////////////////////

//LED引脚初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE); //使能PB，PE端口时钟
	//PB.5端口参数配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                 //PB5端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                          //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         //IO口速度为50MHZ
	GPIO_Init(GPIOB,&GPIO_InitStructure);                                     //初始化PB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_0);                                           //设置PB.5输出高
	//PE.5端口参数配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                                 //PB5端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                          //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         //IO口速度为50MHZ
	GPIO_Init(GPIOE,&GPIO_InitStructure);                                     //初始化PB.5
	GPIO_SetBits(GPIOE,GPIO_Pin_5);                                           //设置PB.5输出高
}

