#include <string.h>

#include "usart1.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"

u8 RX_BUFFER[RX_BUFFER_LEN];//���ܻ�����
u8 TX_BUFFER[TX_BUFFER_LEN];//���ͻ�����

volatile u16 RX_data_len=0;  //���ջ������Ѵ洢�����ݳ���
volatile u16 RX_pos=0;  //���ջ������Ѷ������ֽ�λ��

volatile u16 TX_pos=0; //���ͻ�����ָ��
volatile u16 TX_len=0;//Ҫ���͵����ݳ���
u8 RX_Availiable=0;


void usart1_Init(u32 baud)//����1��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	

	//GPIO��ʼ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//USART1��ʼ��
	USART_InitStruct.USART_BaudRate=baud;//������
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//�������
	USART_InitStruct.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//ֹͣλ1
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//����֡8λ
	USART_Init(USART1,&USART_InitStruct);
	
		
	//GPIOA����USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART1->CR1|=(0x0001<<5);//RXNEIE��1�������ж�
	USART1->CR1|=(0x0001<<3);//TE������ʹ��
	USART1->CR1|=(0x0001<<13);//ʹ�ܴ���

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}



void USART1_IRQHandler(void)//����1�жϷ������
{
	u8 Rec;

	if(USART1->SR&(0x0001<<5))  //RXNEλΪ1�������ж�
	{
		Rec = USART1->DR;//(USART1->DR);	//��ȡ���յ�������
		
		RX_BUFFER[(RX_pos+RX_data_len)%RX_BUFFER_LEN] = Rec;
		RX_data_len++;		
		RX_Availiable  = 1;//��������������
  }
	else if(USART1->SR&(0x0001<<7))  //�������ݼĴ���Ϊ�գ����Է���һ���ֽ�
	{	
		if(TX_pos == TX_len)
		{
			USART1->CR1&=(~(0x0001<<7));//������ϣ��ر�TXE�ж�
			TX_pos=0;
			TX_len=0;
			return;
		}				
		USART1->DR = TX_BUFFER[TX_pos];//�����ͻ���������һ���ֽ�д��
		//ע��TX_pos�ķ�Χ��0~TX_len-1�����TX_pos==TX_lenʱ��˵�����һ���ֽڷ�����
		TX_pos++;
	}
	return;
} 

void usart1_SendByte(u8 data)
{
	USART1->DR=data;
	return;
}

void usart1_SendStr(char* str)
{
	TX_len=strlen(str);
	memcpy(TX_BUFFER,str,TX_len);
	USART1->CR1|=(0x0001<<7);//���俪ʼ����TXE�ж�
	return;
}

void usart1_SendData(u8* data,u8 N)
{
	memcpy(TX_BUFFER,data,N);//�����ݿ��������ͻ�����
	TX_len=N;
	USART1->CR1|=(0x0001<<7);//���俪ʼ����TXE�ж�
	return;
}

u8 usart1_ReadByte(void)
{
	u8 data = RX_BUFFER[RX_pos];
	
	if(RX_data_len>0)
	{
		RX_data_len--;//������һ������
		RX_pos++;
		RX_pos%=RX_BUFFER_LEN;
	}
	if(RX_data_len==0)
	{
		RX_Availiable = 0;//��������������
	}	
	return data;
}
