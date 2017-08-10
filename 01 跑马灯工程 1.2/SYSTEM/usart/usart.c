#include "sys.h"
#include "usart.h"	  

//���ڷ��ͺ���
/*
*p�Ƿ��������׵�ַ
lengthΪ�������ݳ���
*/
void USART1_Send(u8 *p,u8 length)
{ 
	u8 i;
	for(i=0;i<length;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//�ж��Ƿ������
		USART_SendData(USART1, *p);//��������
		p++;
	}
}
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
//���ڳ�ʼ������
void Usart_Init(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//ʹ�����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	//��ʼ������PA.9��PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //����1��TXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������������Ҫʹ�ô��ڹ��ܣ�����ѡ�����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					 //����1��RXD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//USART�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //����1ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure);
	//���ڲ�������
	USART_InitStructure.USART_BaudRate = bound;                                    //���ò�����
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;									 //����Ϊ�շ�ģʽ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //����8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;												 //����1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //��������żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������Ӳ������������
	USART_Init(USART1,&USART_InitStructure);    //��ʼ������
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���������ж�
	USART_Cmd(USART1,ENABLE);										//ʹ�ܴ���
}
//����1�����жϳ���
void USART1_IRQHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		 }   		 
   } 
} 
#endif	

