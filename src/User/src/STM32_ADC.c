/***************************************************
**STM32_ADC.c
***************************************************/  
//#include "STM32Lib\\stm32f10x.h"  
#include "STM32_ADC.h"

#define ADC1_DR_Address    ((u32)0x4001244C) 

unsigned short int ADC1ConvertedValue[20];	 

static DMA_InitTypeDef DMA_InitStructure;

//前提是开启时钟
void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   /* PA0-2 A/D转换  */			
	GPIO_InitStructure.GPIO_Pin = ADC_IO1 | ADC_IO2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure); 	
}

//********************************DMA配置函数***************************************
//函数定义: void DMA_Configuration(void)
//描    述：DMA的配置与初始化
//入口参数：无
//出口参数：无
//备    注：
//**********************************************************************************
void DMA_Configuration(void)
{
//	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);			 //	DMA通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	   //外设地址  Datasheet查
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1ConvertedValue;	 //内存地址,dma取数据的源地址  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA传输方向单向,SRC模式，从外设向内存中搬运
	DMA_InitStructure.DMA_BufferSize =20;					//设置DMA在传输时缓冲区的长度，2路ad,各采样10次，
	//使序列1结果放在AD_Value[0]、AD_Value[2]...，序列2结果放在AD_Value[1]
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 设置DMA的外设递增模式，一个外设,不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	   // 设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// DMA搬运的外设数据字长,注意ADC是12位的，HalfWord是16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//	内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 //	设置DMA的传输模式普通模式DMA_Mode_Normal（需要再次开启）：DMA_Mode_Circular:连续不断的循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	 //置DMA的优先级别 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //	 设置DMA的2个memory中的变量互相访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); // 允许DMA传输完中断

//	ADC_DMACmd(ADC1, ENABLE);	  	/* Enable ADC1 DMA使能ADC1 DMA */
	DMA_Cmd(DMA1_Channel1, ENABLE);	   	/* Enable DMA1 channel1 */ 	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//重新允许DMA,
/*
普通模式DMA_Mode_Normal完成后需要重新打开dma，需要重新配置参数，所以此处将
static DMA_InitTypeDef DMA_InitStructure;为外部变量
//此外不执行软件启动，貌似也进不去中断
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

*/
void DMAReConfig(void)
{
//	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}


//********************************ADC配置函数***************************************
//函数定义: void ADC_Configuration(void)
//描    述：ADC的配置与初始化
//入口参数：无
//出口参数：无
//备    注：
//**********************************************************************************
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立工作模式 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 	//扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发禁止
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	// 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 	//	用于转换的通道数
	ADC_Init(ADC1, &ADC_InitStructure);	  	//

	/* ADC1 regular channels configuration 规则模式通道配置*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 , 1, ADC_SampleTime_41Cycles5);//
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint , 2, ADC_SampleTime_41Cycles5);//
	ADC_TempSensorVrefintCmd(ENABLE);		// 打开温度传感器以及参考电压通道  ADC_Channel_TempSensor

//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2 , 3, ADC_SampleTime_41Cycles5);//

 	ADC_DMACmd(ADC1, ENABLE);	  	/* Enable ADC1 DMA使能ADC1 DMA */
	ADC_Cmd(ADC1, ENABLE);			/* Enable ADC1 */	
	ADC_ResetCalibration(ADC1);		 	/* Enable ADC1 reset calibaration register 下面是ADC自动校准，开机后需执行一次，保证精度*/
	while (ADC_GetResetCalibrationStatus(ADC1));  /* Check the end of ADC1 reset calibration register */
	ADC_StartCalibration(ADC1);	/* Start ADC1 calibaration */
	while (ADC_GetCalibrationStatus(ADC1));	 /* Check the end of ADC1 calibration */

//		/* 人工打开ADC转换.*/ 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/**************************************************************
** 函数名:DigitFilter
** 功能:软件滤波
** 注意事项:取NO的2/5作为头尾忽略值,注意N要大于5,否则不会去头尾
***************************************************************/
unsigned short int DigitFilter(unsigned short int* buf,unsigned short int no)
{
	unsigned char i,j;
	unsigned short int tmp;
	unsigned char cut_no=0;
	//排序
	for(i=0;i<no;i++)
	{
		for(j=0;j<no-i-1;j++)
		{	
			
			if(buf[j]>buf[j+1])
			{	
			tmp=buf[j];
			buf[j]=buf[j+1];
			buf[j+1]=tmp;
			}
		}
	}
	if(no>5)
	{
		cut_no=no/5;
	}

	//平均
	tmp=0;
	for(i=cut_no;i<no-cut_no;i++)	//只取中间n-2*cut_no个求平均
		tmp+=buf[i];
	return(tmp/(no-2*cut_no));
}










