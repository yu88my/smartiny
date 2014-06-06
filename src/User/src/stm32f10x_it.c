/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "System_conf.h"
#include "STM32_RTC.h"//RTC的部分功能
#include "LED.h"

//extern volatile unsigned char ad_flag;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)		  //  SysTick中断服务函数
{
//	Num_count(); 
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void EXTI4_IRQHandler(void)//自己添加的线4中断服务函数
{
}

void EXTI9_5_IRQHandler(void)//自己添加的线4中断服务函数
{
}
//extern u8 Uart1_Get_Flag;
 
void USART1_IRQHandler(void)//自己添加,串口接收中断程序
{
	u8 Uart1_Get_Data; 
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//接收中断	
    {
  		Uart1_Get_Data=USART_ReceiveData(USART1);
			if(Uart1_Get_Data=='s')								//接收到发送数据指令 's' 0X73
		  {
		    LED2_OFF();
		  }
//		Uart1_Get_Flag=1;
//		USART_SendData(USART1, Uart1_Get_Data); //将收到的数据发送回去		
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );  
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	   /* Clear the USART1 Receive interrupt */
    }
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //发送中断	
	{
//		USART_SendData(USART1, 'f'); //将收到的数据发送回去
////		USART_ClearITPendingBit(USART1,USART_IT_TXE);   //发送字节结束
//		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
//		USART_SendData(USART1, USART_ReceiveData(USART1)); //将收到的数据发送回去
	}      
//    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //发送中断
//    {
//		USART_SendData(USART1, Send_Data[Send_Length++]);
//		if (Send_Length==SEND_LENGTH)					 //在需要发送的程序里Send_Data[SEND_LENGTH]和发送长度设置好
//		{     
//			USART_ClearITPendingBit(USART1,USART_IT_TXE);   //发送字节结束
//			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
//		}
//    } 
//    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)   //发送完成
//    {
//		USART_ClearITPendingBit(USART1,USART_IT_TC);
//		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
//	}
	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//读SR
		USART_ReceiveData(USART1);				//读DR
	}
} 


// void DMA1_Channel1_IRQHandler(void)	   
// {
// 	if(DMA_GetITStatus(DMA1_IT_TC1))
//  	{
// 		DMA_ClearITPendingBit(DMA1_IT_GL1);	//清除全部中断标志
// 		ad_flag=1;
// 	}
// }

extern volatile bool RTC_1S;
extern void RTCTick(void);
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET) //RTC发生了秒中断（也有可能是溢出或者闹钟中断)
	{
		RTC_WaitForLastTask();					//--
		RTC_ClearITPendingBit(RTC_IT_SEC);		//清除RTC中断标志位
		RTCTick();								//调用RTC跳动函数.
		RTC_WaitForLastTask();					//--
		//置位RTC秒标志
		RTC_1S=TRUE;
		/* 当计算到24:00:00的时候，要人工归0,*/
		 if(RTC_GetCounter() >= 0x0001517f)	
		{ 
			RTC_SetCounter(0x0);
			//RTC_WaitForLastTask();			//--
		}
	 }
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
