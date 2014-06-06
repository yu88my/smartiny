/***************************************************
**LED.c
***************************************************/

//#include "STM32Lib\\stm32f10x.h"  
#include "KEY.h"
#define KEY1 GPIO_ReadInputDataBit(KEY_PORT, KEY1_IO)
// #define KEY2 GPIO_ReadInputDataBit(KEY_PORT, KEY2_IO)
// #define KEY3 GPIO_ReadInputDataBit(KEY_PORT, KEY3_IO)
// #define KEY4 GPIO_ReadInputDataBit(KEY_PORT, KEY4_IO)
// #define KEY5 GPIO_ReadInputDataBit(KEY_PORT, KEY5_IO)
//#define KEY6 GPIO_ReadInputDataBit(KEY_PORT, KEY6_IO)

//#define KEY7 GPIO_ReadInputDataBit(KEY_PORT1, KEY7_IO)
void delay_key_ms(unsigned short int time)
{    
   unsigned short int i=0;  
   while(time--)
   {
      i=1200;  //尹工用的18000
      while(i--) ;    
   }
}  

void  KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin  = KEY1_IO;	//
//	GPIO_InitStructure.GPIO_Pin  = KEY1_IO | KEY3_IO | KEY4_IO ;	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);

// 	GPIO_InitStructure.GPIO_Pin  = KEY5_IO;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//_IPD 一个采用的是高电平检测按键
// 	GPIO_Init(KEY_PORT, &GPIO_InitStructure);	   
}

unsigned char KEY_Scan(void)
{
	if(!KEY1)
	{
		delay_key_ms(10);
		if(!KEY1)
		return 1;
	}
//	else 
// 	if(!KEY2)
// 	{
// 		delay_key_ms(10);
// 		if(!KEY2)
// 		return 2;
// 	}
// 	else if(!KEY3)
// 	{
// 		delay_key_ms(10);
// 		if(!KEY3)
// 		return 3;
// 	}
// 	else if(!KEY4)
// 	{
// 		delay_key_ms(10);
// 		if(!KEY4)
// 		return 4;
// 	}
// 	else if(KEY5)
// 	{
// 		delay_key_ms(10);
// 		if(KEY5)
// 		return 5;
// 	}
//	else if(!KEY6)
//	{
//		delay_key_ms(10);
//		if(!KEY6)
//		return 6;
//	}
//	else if(!KEY7)
//	{
//		delay_key_ms(10);
//		if(!KEY7)
//		return 7;
//	}	
//	else 
	return 0;
		
}


