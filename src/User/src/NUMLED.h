#ifndef NUMLED_H
#define NUMLED_H

#include "stm32f10x.h"
//Ó²¼þ³õÊ¼»¯
#define NUMLED_PORT GPIOC
#define NUMLED1_IO GPIO_Pin_0    /* PC.0 */
#define NUMLED2_IO GPIO_Pin_1    
#define NUMLED3_IO GPIO_Pin_2
#define NUMLED4_IO GPIO_Pin_3    
#define NUMLED5_IO GPIO_Pin_4    	
#define NUMLED6_IO GPIO_Pin_5    
#define NUMLED7_IO GPIO_Pin_6    	
#define NUMLED8_IO GPIO_Pin_7   

#define NUMLED_C_PORT GPIOC
//#define NUMLED_C_PORT1 GPIOA
#define NUMLED_C_IO1 GPIO_Pin_8
#define NUMLED_C_IO2 GPIO_Pin_9
#define NUMLED_C_IO3 GPIO_Pin_10   
#define NUMLED_C_IO4 GPIO_Pin_11     	   

extern void  NUMLED_Init(void);
extern void NUMLED_Dis(unsigned char num,unsigned char value);   

#endif
