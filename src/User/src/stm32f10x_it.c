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
#include "STM32_RTC.h"//RTC�Ĳ��ֹ���
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
void SysTick_Handler(void)		  //  SysTick�жϷ�����
{
//	Num_count(); 
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void EXTI4_IRQHandler(void)//�Լ���ӵ���4�жϷ�����
{
}

void EXTI9_5_IRQHandler(void)//�Լ���ӵ���4�жϷ�����
{
}
//extern u8 Uart1_Get_Flag;
 
void USART1_IRQHandler(void)//�Լ����,���ڽ����жϳ���
{
	u8 Uart1_Get_Data; 
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)		//�����ж�	
    {
  		Uart1_Get_Data=USART_ReceiveData(USART1);
			if(Uart1_Get_Data=='s')								//���յ���������ָ�� 's' 0X73
		  {
		    LED2_OFF();
		  }
//		Uart1_Get_Flag=1;
//		USART_SendData(USART1, Uart1_Get_Data); //���յ������ݷ��ͻ�ȥ		
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );  
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	   /* Clear the USART1 Receive interrupt */
    }
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //�����ж�	
	{
//		USART_SendData(USART1, 'f'); //���յ������ݷ��ͻ�ȥ
////		USART_ClearITPendingBit(USART1,USART_IT_TXE);   //�����ֽڽ���
//		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
//		USART_SendData(USART1, USART_ReceiveData(USART1)); //���յ������ݷ��ͻ�ȥ
	}      
//    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //�����ж�
//    {
//		USART_SendData(USART1, Send_Data[Send_Length++]);
//		if (Send_Length==SEND_LENGTH)					 //����Ҫ���͵ĳ�����Send_Data[SEND_LENGTH]�ͷ��ͳ������ú�
//		{     
//			USART_ClearITPendingBit(USART1,USART_IT_TXE);   //�����ֽڽ���
//			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
//		}
//    } 
//    if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)   //�������
//    {
//		USART_ClearITPendingBit(USART1,USART_IT_TC);
//		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
//	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ��� �������������жϵ�����
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	//��SR
		USART_ReceiveData(USART1);				//��DR
	}
} 


// void DMA1_Channel1_IRQHandler(void)	   
// {
// 	if(DMA_GetITStatus(DMA1_IT_TC1))
//  	{
// 		DMA_ClearITPendingBit(DMA1_IT_GL1);	//���ȫ���жϱ�־
// 		ad_flag=1;
// 	}
// }

extern volatile bool RTC_1S;
extern void RTCTick(void);
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET) //RTC���������жϣ�Ҳ�п�����������������ж�)
	{
		RTC_WaitForLastTask();					//--
		RTC_ClearITPendingBit(RTC_IT_SEC);		//���RTC�жϱ�־λ
		RTCTick();								//����RTC��������.
		RTC_WaitForLastTask();					//--
		//��λRTC���־
		RTC_1S=TRUE;
		/* �����㵽24:00:00��ʱ��Ҫ�˹���0,*/
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
