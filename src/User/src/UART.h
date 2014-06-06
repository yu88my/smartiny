#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
//硬件初始化

#define Baud_Rate 9600	   		  //波特率设置

extern void UART1_GPIO_Configuration(void); 
extern void USART_Configuration(void);
extern void USART1_Putc(unsigned char c); //发送字符函数
extern void USART1_Puts(char * str);	//发送字符串函数 
				 
#endif
