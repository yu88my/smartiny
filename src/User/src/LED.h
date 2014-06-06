#ifndef LED_H
#define LED_H

#include "stm32f10x.h"
//Ó²¼þ³õÊ¼»¯
#define LED_PORT1 GPIOC
#define LED1_IO   GPIO_Pin_1      /* PC.1 */

	   
#define LED_PORT2 GPIOD
#define LED2_IO   GPIO_Pin_9    /* PD.9 */

extern void  LED_Init(void);

// extern void  BELL_Init(void);
// extern void  BELL_ON(void);
// extern void  BELL_OFF(void);

extern void  LED1_ON(void);
extern void  LED1_OFF(void);
extern void  LED2_ON(void);
extern void  LED2_OFF(void);
//extern void  LED3_ON(void);
//extern void  LED3_OFF(void);   

#endif
