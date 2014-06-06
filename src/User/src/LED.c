/***************************************************
**LED.c
***************************************************/

//#include "STM32Lib\\stm32f10x.h"  
#include "LED.h"

//ǰ���Ƕ�����ʱ��
void  LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	  
//�ı�ָ���ܽŵ�ӳ��  GPIO_Remap_SWJ_JTAGDisable  JTAG-DP ʧ�� + SW-DPʹ��
//	AFIO->MAPR = 0x02000000; //�ͷų�����JTAG�Ŀ�PA15,PB3,PB4,����ͨIO���� ����SW���ߵ��Կ�
	GPIO_InitStructure.GPIO_Pin  = LED1_IO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = LED2_IO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT2, &GPIO_InitStructure);
	
//	GPIO_SetBits(LED_PORT, LED1_IO | LED2_IO );	//��ȫ������ߣ��Ʋ���
}


void  LED1_ON(void)
{
	GPIO_ResetBits(LED_PORT1, LED1_IO);
}
void  LED1_OFF(void)
{
	GPIO_SetBits(LED_PORT1, LED1_IO );
}
void  LED2_ON(void)
{
	GPIO_ResetBits(LED_PORT2, LED2_IO);
}
void  LED2_OFF(void)
{
	GPIO_SetBits(LED_PORT2, LED2_IO );
}
//void  LED3_ON(void)
//{
//	GPIO_ResetBits(LED_PORT, LED3_IO);
//}
//void  LED3_OFF(void)
//{
//	GPIO_SetBits(LED_PORT, LED3_IO);
//}

// void  BELL_Init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;   
// 	GPIO_InitStructure.GPIO_Pin  = BELL_IO ;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
// 	GPIO_Init(BELL_PORT, &GPIO_InitStructure); 	
// 	GPIO_SetBits(BELL_PORT, BELL_IO);	//�Ȳ�����ߣ�����������
// }
// void  BELL_ON(void)
// {
// 	GPIO_ResetBits(BELL_PORT, BELL_IO);	//����ͣ��������� 
// }
// void  BELL_OFF(void)
// {
// 	GPIO_SetBits(BELL_PORT, BELL_IO);	//����ߣ�����������
// }


