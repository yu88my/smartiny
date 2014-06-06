/***************************************************
**LED.c
***************************************************/ 
//#include "STM32Lib\\stm32f10x.h"  
#include "System_conf.h"

#include "NUMLED.h"

volatile unsigned int Sys_Delay;
//extern 
//volatile unsigned char Num_LED_Count;
//extern unsigned char disnum[4];

void delay_nms(unsigned short int time)
{    
   unsigned short int i=0;  
   while(time--)
   {
      i=1200;  //ʵ�⡣8M�����
      while(i--) ;    
   }
}
void delay_nus(unsigned short int time)
{    
   unsigned short int i=0;  
   while(time--)
   {
      i=2;  //�����õ�17 ,ʵ��Ϊ2��8M�����
      while(i--) ;    
   }
}
 
void Sys_Clock_Init(void)
{
	ErrorStatus HSEStartUpStatus; 
	RCC_DeInit(); //     	/* RCC system reset(for debug purpose) ������RCC������*/
	RCC_HSEConfig(RCC_HSE_ON);		/* Enable HSE */  		/* ʹ���ⲿ���پ��� */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();	  /* Wait till HSE is ready �ȴ����پ����ȶ�*/ 	
	if(HSEStartUpStatus == SUCCESS)
	{
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	 	/* Enable Prefetch Buffer ʹ��flashԤ��ȡ������*/  		
//		FLASH_SetLatency(FLASH_Latency_2);		  /* Flash 2 wait state 
//		��Flash���ڵȴ�״̬��2����Ը�Ƶʱ�ӵģ��������RCCûֱ�ӹ�ϵ�����������Թ�*/	   	
		RCC_HCLKConfig(RCC_SYSCLK_Div1);		  	/* HCLK = SYSCLK ���ø�������ʱ��=ϵͳʱ��*/	   	
		RCC_PCLK2Config(RCC_HCLK_Div1);			  	/* PCLK2 = HCLK ���õ�������2ʱ��=��������ʱ��*/  		
		RCC_PCLK1Config(RCC_HCLK_Div2);				/* PCLK1 = HCLK/2 ���õ�������1��ʱ��=����ʱ�ӵĶ���Ƶ*/  
		 	
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);		   	/* ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ*/	

/*		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  // PLLCLK = 8MHz * 9 = 72 MHz �������໷���ⲿ8Mhz����9��Ƶ��72Mhz 	 
		RCC_PLLCmd(ENABLE);			// Enable PLL ʹ�����໷		 																
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)		// Wait till PLL is ready �ȴ����໷����ȶ�
		{
		} */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
		//RCC_SYSCLKSource_HSI:	�����ڲ�ʱ���ź� stm32��Ƭ���ڴ���ʱ�� (8MƵ��)	 
		//RCC_SYSCLKSource_HSE:(1)HSE�ⲿ����/�մ�г����(����)  (2)HSE�û��ⲿʱ��
		//RCC_SYSCLKSource_PLLCLK:Select PLL as system clock source �����໷�������Ϊϵͳʱ��*/	
		while(RCC_GetSYSCLKSource() != 0x04)	//HSE used as system clock
// 		while(RCC_GetSYSCLKSource() != 0x08)	/* Wait till PLL is used as system clock source �ȴ�PLLУ��ɹ�*/
		{
		}
	}
	/* ������ÿ�ʹ�ⲿ����ͣ���ʱ��,����һ��NMI�ж�,����Ҫ�õĿ����ε�*/
	//RCC_ClockSecuritySystemCmd(ENABLE);
	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    //ʹ����Χ�ӿ�����ʱ�ӣ�ע�������������������ͬоƬ�ķ��䲻ͬ�����ֲ�Ϳ�
	/* Enable peripheral clocks -------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
//                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
//                         RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  // AFIOʱ�ӣ�������ֹJTAG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	  // ����1ʱ�ӣ�
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);     //ADC1ʱ�� 
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ʱ�� 	
} 

void NVIC_Configuration(void)	   //�жϿ��������ú���
{
	NVIC_InitTypeDef NVIC_InitStructure;	   //�жϹ���ָ�Ĭ�ϲ��� 

	#ifdef  VECT_TAB_RAM   
	//���C/C++ Compiler\Preprocessor\Defined symbols�еĶ�����VECT_TAB_RAM���������������ݵı�� 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); //����RAM���� 
	#else        //���û�ж���VECT_TAB_RAM 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//����Flash����� 
	#endif        //�����ж����	
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
//����NVIC���ȼ����飬��ʽ�� 
//ע��һ��16�����ȼ�����Ϊ��ռʽ����Ӧʽ���������ȼ���ռ�������ɴ˴���ȷ��.
//NVIC_PriorityGroup_x������0��1�� 2��3��4���ֱ������ռ���ȼ���1��2��4��8��16������Ӧ���ȼ���16��8��4��2��1����
//�涨�������ȼ������������е��жϼ������������ѡ��
//��ռ����ߵĻ��������ж�����ִ�У�����Ӧ����ߵĻ��������ж�ִ��������ִ�С� 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* �����ж�ʹ�����0*/			
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	 //�ж����Ƽ�������stm32f10x.h,���õ�����4

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //��Ӧ���ȼ� 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		  //������ͨ�����ж� 
	NVIC_Init(&NVIC_InitStructure);						  //�жϳ�ʼ��
 
// 	/* Enable DMA channel6 IRQ Channel */
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);

	/*RTC*/
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_SetPriority(SysTick_IRQn, 1);                // SysTick��ʱ��the highest priority */

}

void SysTick_Configuration(void)   //SysTick ���ú���,���д˺������ɴ�SysTick�жϣ���������NVIC���������ȼ�
{
	if (SysTick_Config(8000000 / 1000))  //ÿ���в���1000��ϵͳ�δ��жϡ�ǰ��ΪSystemCoreClock�������Ϊÿ������жϴ���  
    {  
       /* Capture error */  
       while (1);  
    } 
}

void Sys_nDelay(volatile unsigned int nTime)	 //Delay: //ϵͳ��ʱ����, ʹ��ϵͳʱ�Ӳ���.
{
	Sys_Delay = nTime; 
	while (Sys_Delay != 0);
}
// void Num_count(void)		 //��SysTick�жϷ����������У�������ʾ��RTC��ȡ
// {
// 	Num_LED_Count++;
// 	if (Sys_Delay != 0x00)
// 	{
// 		Sys_Delay--;	   //�õ�
// 	}
// 	if(Num_LED_Count==4)
// 	{  	NUMLED_Dis(4,disnum[0]);}
// 	if(Num_LED_Count==8)
// 	{	NUMLED_Dis(3,disnum[1]);}
// 	if(Num_LED_Count==12)
// 	{	NUMLED_Dis(2,disnum[2]);}
// 	if(Num_LED_Count==16)
// 	{	
// 		NUMLED_Dis(1,disnum[3]); 
// 		Num_LED_Count=0; 
// 	} 
// }






