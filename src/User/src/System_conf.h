#ifndef System_conf_H
#define System_conf_H

#include "stm32f10x.h"



//Ӳ����ʼ��
extern void Sys_Clock_Init(void);
extern void NVIC_Configuration(void);	   //�жϿ��������ú���
extern void delay_nms(unsigned short int time);
extern void delay_nus(unsigned short int time);

extern void SysTick_Configuration(void);   //SysTick ʱ�����ú���


extern void Sys_nDelay(volatile unsigned int nTime); 		 //Delay: //ϵͳ��ʱ����, ʹ��ϵͳʱ�Ӳ���. 
extern void Num_count(void); //



#endif
