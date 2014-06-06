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
      i=1200;  //实测。8M情况下
      while(i--) ;    
   }
}
void delay_nus(unsigned short int time)
{    
   unsigned short int i=0;  
   while(time--)
   {
      i=2;  //尹工用的17 ,实测为2，8M情况下
      while(i--) ;    
   }
}
 
void Sys_Clock_Init(void)
{
	ErrorStatus HSEStartUpStatus; 
	RCC_DeInit(); //     	/* RCC system reset(for debug purpose) 重置了RCC的设置*/
	RCC_HSEConfig(RCC_HSE_ON);		/* Enable HSE */  		/* 使能外部高速晶振 */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();	  /* Wait till HSE is ready 等待高速晶振稳定*/ 	
	if(HSEStartUpStatus == SUCCESS)
	{
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	 	/* Enable Prefetch Buffer 使能flash预读取缓冲区*/  		
//		FLASH_SetLatency(FLASH_Latency_2);		  /* Flash 2 wait state 
//		令Flash处于等待状态，2是针对高频时钟的，这两句跟RCC没直接关系，可以暂且略过*/	   	
		RCC_HCLKConfig(RCC_SYSCLK_Div1);		  	/* HCLK = SYSCLK 设置高速总线时钟=系统时钟*/	   	
		RCC_PCLK2Config(RCC_HCLK_Div1);			  	/* PCLK2 = HCLK 设置低速总线2时钟=高速总线时钟*/  		
		RCC_PCLK1Config(RCC_HCLK_Div2);				/* PCLK1 = HCLK/2 设置低速总线1的时钟=高速时钟的二分频*/  
		 	
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);		   	/* ADCCLK = PCLK2/6 设置ADC外设时钟=低速总线2时钟的六分频*/	

/*		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  // PLLCLK = 8MHz * 9 = 72 MHz 利用锁相环讲外部8Mhz晶振9倍频到72Mhz 	 
		RCC_PLLCmd(ENABLE);			// Enable PLL 使能锁相环		 																
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)		// Wait till PLL is ready 等待锁相环输出稳定
		{
		} */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
		//RCC_SYSCLKSource_HSI:	高速内部时钟信号 stm32单片机内带的时钟 (8M频率)	 
		//RCC_SYSCLKSource_HSE:(1)HSE外部晶体/陶瓷谐振器(晶振)  (2)HSE用户外部时钟
		//RCC_SYSCLKSource_PLLCLK:Select PLL as system clock source 将锁相环输出设置为系统时钟*/	
		while(RCC_GetSYSCLKSource() != 0x04)	//HSE used as system clock
// 		while(RCC_GetSYSCLKSource() != 0x08)	/* Wait till PLL is used as system clock source 等待PLL校验成功*/
		{
		}
	}
	/* 这个配置可使外部晶振停振的时候,产生一个NMI中断,不需要用的可屏蔽掉*/
	//RCC_ClockSecuritySystemCmd(ENABLE);
	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    //使能外围接口总线时钟，注意各外设的隶属情况，不同芯片的分配不同，查手册就可
	/* Enable peripheral clocks -------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 	
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
//                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
//                         RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	  // AFIO时钟，用来禁止JTAG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	  // 串口1时钟，
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);     //ADC1时钟 
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA1时钟 	
} 

void NVIC_Configuration(void)	   //中断控制器配置函数
{
	NVIC_InitTypeDef NVIC_InitStructure;	   //中断管理恢复默认参数 

	#ifdef  VECT_TAB_RAM   
	//如果C/C++ Compiler\Preprocessor\Defined symbols中的定义了VECT_TAB_RAM（见程序库更改内容的表格） 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); //则在RAM调试 
	#else        //如果没有定义VECT_TAB_RAM 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//则在Flash里调试 
	#endif        //结束判断语句	
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
//设置NVIC优先级分组，方式。 
//注：一共16个优先级，分为抢占式和响应式。两种优先级所占的数量由此代码确定.
//NVIC_PriorityGroup_x可以是0、1、 2、3、4，分别代表抢占优先级有1、2、4、8、16个和响应优先级有16、8、4、2、1个。
//规定两种优先级的数量后，所有的中断级别必须在其中选择，
//抢占级别高的会打断其他中断优先执行，而响应级别高的会在其他中断执行完优先执行。 
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	/* 配置中断使用组合0*/			
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	 //中断名称见函数库stm32f10x.h,采用的是线4

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //响应优先级 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		  //启动此通道的中断 
	NVIC_Init(&NVIC_InitStructure);						  //中断初始化
 
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

	NVIC_SetPriority(SysTick_IRQn, 1);                // SysTick定时器the highest priority */

}

void SysTick_Configuration(void)   //SysTick 配置函数,运行此函数即可打开SysTick中断，此外需在NVIC中申明优先级
{
	if (SysTick_Config(8000000 / 1000))  //每秒中产生1000次系统滴答中断。前面为SystemCoreClock，后面的为每秒进入中断次数  
    {  
       /* Capture error */  
       while (1);  
    } 
}

void Sys_nDelay(volatile unsigned int nTime)	 //Delay: //系统延时函数, 使用系统时钟操作.
{
	Sys_Delay = nTime; 
	while (Sys_Delay != 0);
}
// void Num_count(void)		 //在SysTick中断服务函数中运行，进行显示和RTC读取
// {
// 	Num_LED_Count++;
// 	if (Sys_Delay != 0x00)
// 	{
// 		Sys_Delay--;	   //用到
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






