#ifndef System_conf_H
#define System_conf_H

#include "stm32f10x.h"



//硬件初始化
extern void Sys_Clock_Init(void);
extern void NVIC_Configuration(void);	   //中断控制器配置函数
extern void delay_nms(unsigned short int time);
extern void delay_nus(unsigned short int time);

extern void SysTick_Configuration(void);   //SysTick 时钟配置函数


extern void Sys_nDelay(volatile unsigned int nTime); 		 //Delay: //系统延时函数, 使用系统时钟操作. 
extern void Num_count(void); //



#endif
