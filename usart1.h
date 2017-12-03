#ifndef __USART1_H
#define __USART1_H

#include "stm32f4xx.h"

#include <stdio.h>



#define RX_BUFFER_LEN 256
#define TX_BUFFER_LEN 256

   

extern u8 RX_BUFFER[RX_BUFFER_LEN];//���ܻ�����
extern u8 TX_BUFFER[TX_BUFFER_LEN];//���ͻ�����

extern u8 RX_Availiable; //����������������ʱ���˱���Ϊ1

void usart1_Init(u32);
//����1��ʼ��

void usart1_SendByte(u8);
//����һ���ֽ�

void usart1_SendStr(char*);
//����һ���ַ�������\0��β

void usart1_SendData(u8*,u8);
//����һ��һ�����ȵ����ݰ���������256�ֽڡ�

u8 usart1_ReadByte(void);
//��ȡ�������е�һ���ֽڣ���RX_AcceptableΪ1ʱ�ٵ��ô˺���


#endif


