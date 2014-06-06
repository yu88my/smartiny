/***************************************************
**LED.c
***************************************************/ 
//#include "STM32Lib\\stm32f10x.h"  
#include "UART.h"
//���뿪��ʱ�Ӳ����жϷ�������д
void UART1_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    // Configure USART1_Tx as alternate push-pull 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	   //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
    // Configure USART1_Rx as input floating 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	  //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;	  //���崮�ڳ�ʼ���ṹ�� 
	USART_ClockInitTypeDef  USART_ClockInitStructure;

  	UART1_GPIO_Configuration();	   //
  	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;	 //����ʱ�ӽ�ֹ
  	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;		 //ʱ���½�����Ч
  	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;		 //�����ڵڶ���ʱ���ز�׽
  	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	 //�������λ��ʱ�����岻�����SCLK����   	
  	USART_ClockInit(USART1, &USART_ClockInitStructure);	   /* Configure the USART1 synchronous paramters */

  	USART_InitStructure.USART_BaudRate = Baud_Rate;				  //������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8λ����
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   //1��ֹͣλ
  	USART_InitStructure.USART_Parity = USART_Parity_No ;		//��У��λ
  	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None; //����RTSCTSӲ��������

  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  //ʹ�ܷ��ͽ���    	
  	USART_Init(USART1, &USART_InitStructure);	     	/* Configure USART1 basic and asynchronous paramters */
  	   
  	USART_Cmd(USART1, ENABLE);		/* Enable USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	   /* ʹ�ܴ����ж�Enable USART1 Receive interrupts */ 
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	   /* ʹ�ܷ����ж�Enable USART1 Receive interrupts */ ����ʱ����ص�
}

void USART1_Putc(unsigned char c) //�����ַ�����
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}
void USART1_Puts(char * str)	//�����ַ�������
{
    while(*str)
    {
        USART_SendData(USART1, *str++);	 /* Loop until the end of transmission */         
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
} 






