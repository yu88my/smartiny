#ifndef UART_H
#define UART_H

#include "stm32f10x.h"
//Ӳ����ʼ��

#define Baud_Rate 9600	   		  //����������

extern void UART1_GPIO_Configuration(void); 
extern void USART_Configuration(void);
extern void USART1_Putc(unsigned char c); //�����ַ�����
extern void USART1_Puts(char * str);	//�����ַ������� 
				 
#endif
