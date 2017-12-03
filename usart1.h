#ifndef __USART1_H
#define __USART1_H

#include "stm32f4xx.h"

#include <stdio.h>



#define RX_BUFFER_LEN 256
#define TX_BUFFER_LEN 256

   

extern u8 RX_BUFFER[RX_BUFFER_LEN];//接受缓冲区
extern u8 TX_BUFFER[TX_BUFFER_LEN];//发送缓冲区

extern u8 RX_Availiable; //当缓冲区内有数据时，此变量为1

void usart1_Init(u32);
//串口1初始化

void usart1_SendByte(u8);
//发送一个字节

void usart1_SendStr(char*);
//发送一个字符串，以\0结尾

void usart1_SendData(u8*,u8);
//发送一个一定长度的数据包，不超过256字节。

u8 usart1_ReadByte(void);
//读取缓冲区中的一个字节，在RX_Acceptable为1时再调用此函数


#endif


