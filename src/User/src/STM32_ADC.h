#ifndef STM32_ADC_H
#define STM32_ADC_H

#include "stm32f10x.h" 

//Ó²¼þ³õÊ¼»¯

#define ADC_PORT  GPIOA
#define ADC_IO1   GPIO_Pin_0    /* PA0 */	   
#define ADC_IO2   GPIO_Pin_1  
#define ADC_IO3   GPIO_Pin_2  

//unsigned short int ADC1ConvertedValue[20];	 
extern unsigned short int ADC1_Value1,ADC1_Value2,ADC1_Value3; 

extern void DMA_Configuration(void);
extern void DMAReConfig(void); 
extern void ADC_Configuration(void);  
extern void ADC_GPIO_Configuration(void); 
extern unsigned short int DigitFilter(unsigned short int* buf,unsigned short int no);   
				 
#endif
