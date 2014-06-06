/***************************************************
**STM32_ADC.c
***************************************************/  
//#include "STM32Lib\\stm32f10x.h"  
#include "STM32_ADC.h"

#define ADC1_DR_Address    ((u32)0x4001244C) 

unsigned short int ADC1ConvertedValue[20];	 

static DMA_InitTypeDef DMA_InitStructure;

//ǰ���ǿ���ʱ��
void ADC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   /* PA0-2 A/Dת��  */			
	GPIO_InitStructure.GPIO_Pin = ADC_IO1 | ADC_IO2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure); 	
}

//********************************DMA���ú���***************************************
//��������: void DMA_Configuration(void)
//��    ����DMA���������ʼ��
//��ڲ�������
//���ڲ�������
//��    ע��
//**********************************************************************************
void DMA_Configuration(void)
{
//	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);			 //	DMAͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	   //�����ַ  Datasheet��
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1ConvertedValue;	 //�ڴ��ַ,dmaȡ���ݵ�Դ��ַ  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //DMA���䷽����,SRCģʽ�����������ڴ��а���
	DMA_InitStructure.DMA_BufferSize =20;					//����DMA�ڴ���ʱ�������ĳ��ȣ�2·ad,������10�Σ�
	//ʹ����1�������AD_Value[0]��AD_Value[2]...������2�������AD_Value[1]
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // ����DMA���������ģʽ��һ������,������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	   // ����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// DMA���˵����������ֳ�,ע��ADC��12λ�ģ�HalfWord��16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//	�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 //	����DMA�Ĵ���ģʽ��ͨģʽDMA_Mode_Normal����Ҫ�ٴο�������DMA_Mode_Circular:�������ϵ�ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	 //��DMA�����ȼ��� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //	 ����DMA��2��memory�еı����������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); // ����DMA�������ж�

//	ADC_DMACmd(ADC1, ENABLE);	  	/* Enable ADC1 DMAʹ��ADC1 DMA */
	DMA_Cmd(DMA1_Channel1, ENABLE);	   	/* Enable DMA1 channel1 */ 	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//��������DMA,
/*
��ͨģʽDMA_Mode_Normal��ɺ���Ҫ���´�dma����Ҫ�������ò��������Դ˴���
static DMA_InitTypeDef DMA_InitStructure;Ϊ�ⲿ����
//���ⲻִ�����������ò��Ҳ����ȥ�ж�
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


//********************************ADC���ú���***************************************
//��������: void ADC_Configuration(void)
//��    ����ADC���������ʼ��
//��ڲ�������
//���ڲ�������
//��    ע��
//**********************************************************************************
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//��������ģʽ 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 	//ɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ������ֹ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	// �����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 	//	����ת����ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);	  	//

	/* ADC1 regular channels configuration ����ģʽͨ������*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 , 1, ADC_SampleTime_41Cycles5);//
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint , 2, ADC_SampleTime_41Cycles5);//
	ADC_TempSensorVrefintCmd(ENABLE);		// ���¶ȴ������Լ��ο���ѹͨ��  ADC_Channel_TempSensor

//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2 , 3, ADC_SampleTime_41Cycles5);//

 	ADC_DMACmd(ADC1, ENABLE);	  	/* Enable ADC1 DMAʹ��ADC1 DMA */
	ADC_Cmd(ADC1, ENABLE);			/* Enable ADC1 */	
	ADC_ResetCalibration(ADC1);		 	/* Enable ADC1 reset calibaration register ������ADC�Զ�У׼����������ִ��һ�Σ���֤����*/
	while (ADC_GetResetCalibrationStatus(ADC1));  /* Check the end of ADC1 reset calibration register */
	ADC_StartCalibration(ADC1);	/* Start ADC1 calibaration */
	while (ADC_GetCalibrationStatus(ADC1));	 /* Check the end of ADC1 calibration */

//		/* �˹���ADCת��.*/ 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/**************************************************************
** ������:DigitFilter
** ����:����˲�
** ע������:ȡNO��2/5��Ϊͷβ����ֵ,ע��NҪ����5,���򲻻�ȥͷβ
***************************************************************/
unsigned short int DigitFilter(unsigned short int* buf,unsigned short int no)
{
	unsigned char i,j;
	unsigned short int tmp;
	unsigned char cut_no=0;
	//����
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

	//ƽ��
	tmp=0;
	for(i=cut_no;i<no-cut_no;i++)	//ֻȡ�м�n-2*cut_no����ƽ��
		tmp+=buf[i];
	return(tmp/(no-2*cut_no));
}










