/***************************************************
**LED.c
***************************************************/ 
//#include "STM32Lib\\stm32f10x.h"  
#include "UART.h"
//必须开启时钟并在中断服务函数书写
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
	USART_InitTypeDef USART_InitStructure;	  //定义串口初始化结构体 
	USART_ClockInitTypeDef  USART_ClockInitStructure;

  	UART1_GPIO_Configuration();	   //
  	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;	 //串口时钟禁止
  	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;		 //时钟下降沿有效
  	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;		 //数据在第二个时钟沿捕捉
  	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	 //最后数据位的时钟脉冲不输出到SCLK引脚   	
  	USART_ClockInit(USART1, &USART_ClockInitStructure);	   /* Configure the USART1 synchronous paramters */

  	USART_InitStructure.USART_BaudRate = Baud_Rate;				  //波特率
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8位数据
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;	   //1个停止位
  	USART_InitStructure.USART_Parity = USART_Parity_No ;		//无校验位
  	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None; //禁用RTSCTS硬件流控制

  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	  //使能发送接收    	
  	USART_Init(USART1, &USART_InitStructure);	     	/* Configure USART1 basic and asynchronous paramters */
  	   
  	USART_Cmd(USART1, ENABLE);		/* Enable USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	   /* 使能串口中断Enable USART1 Receive interrupts */ 
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	   /* 使能发送中断Enable USART1 Receive interrupts */ 不用时必须关掉
}

void USART1_Putc(unsigned char c) //发送字符函数
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}
void USART1_Puts(char * str)	//发送字符串函数
{
    while(*str)
    {
        USART_SendData(USART1, *str++);	 /* Loop until the end of transmission */         
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
} 






