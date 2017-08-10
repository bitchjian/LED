#include "sys.h"
#include "usart.h"	  

//串口发送函数
/*
*p是发送数据首地址
length为发送数据长度
*/
void USART1_Send(u8 *p,u8 length)
{ 
	u8 i;
	for(i=0;i<length;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//判断是否发送完成
		USART_SendData(USART1, *p);//发送数据
		p++;
	}
}
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
//串口初始化函数
void Usart_Init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//使能相关时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	//初始化引脚PA.9和PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //串口1的TXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出，由于要使用串口功能，所以选择这个模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					 //串口1的RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //串口1通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //使能IRQ通道
	NVIC_Init(&NVIC_InitStructure);
	//串口参数配置
	USART_InitStructure.USART_BaudRate = bound;                                    //设置波特率
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									 //设置为收发模式
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //设置8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;												 //设置1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //设置无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置无硬件数据流控制
	USART_Init(USART1,&USART_InitStructure);    //初始化串口
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启串口中断
	USART_Cmd(USART1,ENABLE);										//使能串口
}
//串口1服务中断程序
void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		 }   		 
   } 
} 
#endif	

