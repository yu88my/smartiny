#include "stm32f10x.h"
#include "System_conf.h"	
#include "LED.h"
#include "KEY.h"
//#include "NUMLED.h"
#include "UART.h"
//#include "I2C_Driver.h"
//#include "FM1702SL.h"
//#include "iso14443a.h"
//#include "NRF24l01.h" 
//#include "STM32_ADC.h"
#include "STM32_RTC.h"

//volatile unsigned char Num_LED_Count;
unsigned char time[3];

volatile unsigned char mode;	//ģʽ����

volatile bool RTC_1S=FALSE;

void CheckRTC(void); 
//void PrintfRTC(void);
void UpdateRTC(void);


void sys_power_on(void);
void sys_power_off(void);
void bell_play(void);  //��������һ������

int main(void)
{	
	unsigned char key;
	Sys_Clock_Init(); 		 
	SysTick_Configuration();//Sys��ʱ��		
 	USART_Configuration();	//���ô���

	NVIC_Configuration(); 	//�ж�����

//	sys_power_on();//����ϵͳ��Դ
	LED_Init();		//ָʾ��
//	KEY_Init();		//����

	USART1_Puts("this is a uart test!\n"); 

	
	CheckRTC();	   	//2.RTC
	LED2_OFF();

	while (1)
	{
		LED1_OFF();delay_nms(3000);
		LED1_ON();		delay_nms(100);
	  key=KEY_Scan();
		if(key==1)
		{LED2_ON();		}
// 		
// 		 if(mode==0)	  //��ʾʱ��
// 		 {
		 if(RTC_1S==TRUE)
			{
				RTC_1S=FALSE;
				UpdateRTC();
			}
// 		}

	}						                                                                         
}


void CheckRTC(void)
{
	T_STRUCT time;
	if(RTCInit()==FALSE)
	{
		time.year=DEFAULT_TIME_YEAR;
		time.month=DEFAULT_TIME_MONTH;
		time.day=DEFAULT_TIME_DAY;
		time.hour=DEFAULT_TIME_HOUR;
		time.minute=DEFAULT_TIME_MINUTE;
		time.sec=DEFAULT_TIME_SECOND;
		SetRTCTime(&time);
	} 
}

/**************************************
**������:UpdateRTC
**����:��LCD��ˢ��RTC
**ע������:��һ���ļ���������ʾRTC,ʹ��LCD���ظ�ˢдһ����ͬ������
**************************************/
void UpdateRTC(void)
{
	T_STRUCT time;
	GetRTCTime(&time);

	//ˢ����
	USART1_Putc(time.sec);
	//ˢ�·�
	if(time.sec==0)
	{
		USART1_Putc(time.minute);		
		//ˢ��ʱ
		if(time.minute==0)
		{
			USART1_Putc(time.hour);			
			//ˢ����
			if(time.hour==0)
			{
				USART1_Putc(time.day); 				
				//ˢ����
				if(time.day==1)
				{
					USART1_Putc(time.month);					
					//ˢ����
					if(time.month==1)
					{
						USART1_Putc(time.year);
					}
				}
			}
		}
	}
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif 
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
