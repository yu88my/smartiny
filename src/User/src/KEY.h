#ifndef KEY_H
#define KEY_H

#include "stm32f10x.h"
//Ó²¼þ³õÊ¼»¯
#define KEY_PORT GPIOC
#define KEY1_IO GPIO_Pin_0    /* PC.0 */
//#define KEY2_IO GPIO_Pin_0    
// #define KEY3_IO GPIO_Pin_4    	
// #define KEY4_IO GPIO_Pin_5    
// #define KEY5_IO GPIO_Pin_6    
//#define KEY6_IO GPIO_Pin_7    /* PB.7 */	 

//#define KEY_PORT1 GPIOC
//#define KEY7_IO GPIO_Pin_12    /* PC.12 */	   

extern void  KEY_Init(void);  
extern unsigned char KEY_Scan(void);  

void delay_key_ms(unsigned short int time);
				 
#endif
