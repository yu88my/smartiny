/***************************************************
**NUMLED.c
***************************************************/
//#include "STM32Lib\\stm32f10x.h"  
#include "NUMLED.h"

#define NUMLED1_ON() 	GPIO_ResetBits(NUMLED_C_PORT, NUMLED_C_IO1)	//�������
#define NUMLED1_OFF() 	GPIO_SetBits(NUMLED_C_PORT, NUMLED_C_IO1)	//����ܲ���
#define NUMLED2_ON() 	GPIO_ResetBits(NUMLED_C_PORT, NUMLED_C_IO2)	//�������
#define NUMLED2_OFF() 	GPIO_SetBits(NUMLED_C_PORT, NUMLED_C_IO2)	//����ܲ���
#define NUMLED3_ON() 	GPIO_ResetBits(NUMLED_C_PORT, NUMLED_C_IO3)	//�������
#define NUMLED3_OFF() 	GPIO_SetBits(NUMLED_C_PORT, NUMLED_C_IO3)	//����ܲ���
#define NUMLED4_ON() 	GPIO_ResetBits(NUMLED_C_PORT, NUMLED_C_IO4)	//�������
#define NUMLED4_OFF() 	GPIO_SetBits(NUMLED_C_PORT, NUMLED_C_IO4)	//����ܲ���

#define NUMLED_0() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED7_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED3_IO | NUMLED4_IO |NUMLED5_IO | NUMLED6_IO );}	//
#define NUMLED_1() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED6_IO | NUMLED8_IO | NUMLED1_IO | NUMLED4_IO |NUMLED5_IO | NUMLED7_IO );\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED2_IO | NUMLED3_IO);}	//
#define NUMLED_2() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED6_IO | NUMLED3_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED4_IO |NUMLED5_IO | NUMLED7_IO );}	//
#define NUMLED_3() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED6_IO |NUMLED5_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED3_IO | NUMLED4_IO | NUMLED7_IO );}	//
#define NUMLED_4() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED4_IO |NUMLED5_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED2_IO | NUMLED3_IO | NUMLED6_IO | NUMLED7_IO );}	//
#define NUMLED_5() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED5_IO| NUMLED2_IO  | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED3_IO | NUMLED4_IO |NUMLED6_IO | NUMLED7_IO );}	//
#define NUMLED_6() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED2_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED3_IO | NUMLED4_IO |NUMLED5_IO | NUMLED6_IO | NUMLED7_IO );}	//
#define NUMLED_7() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED6_IO | NUMLED8_IO | NUMLED4_IO |NUMLED5_IO | NUMLED7_IO );\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED3_IO);}	//
#define NUMLED_8() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED3_IO | NUMLED4_IO |NUMLED5_IO | NUMLED6_IO | NUMLED7_IO );}	//
#define NUMLED_9() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED5_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED2_IO | NUMLED3_IO | NUMLED4_IO |NUMLED6_IO | NUMLED7_IO );}	//
#define NUMLED_F() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED2_IO | NUMLED3_IO | NUMLED4_IO | NUMLED8_IO);\
                     GPIO_ResetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED5_IO | NUMLED6_IO | NUMLED7_IO );}	// 
#define NUMLED_NULL() 	{GPIO_SetBits(NUMLED_C_PORT, NUMLED2_IO | NUMLED3_IO | NUMLED4_IO | NUMLED8_IO);\
                     GPIO_SetBits(NUMLED_C_PORT, NUMLED1_IO | NUMLED5_IO | NUMLED6_IO | NUMLED7_IO );}	// 

//ǰ���Ƕ�����ʱ��
void  NUMLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	GPIO_InitStructure.GPIO_Pin  = NUMLED1_IO | NUMLED2_IO | NUMLED3_IO | NUMLED4_IO |	 
                                   NUMLED5_IO | NUMLED6_IO | NUMLED7_IO | NUMLED8_IO;	 //�˶�����ܹܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
	GPIO_Init(NUMLED_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = NUMLED_C_IO1 | NUMLED_C_IO2 | NUMLED_C_IO3 | NUMLED_C_IO4;		  //�����˶�����ܿ��ƹܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
	GPIO_Init(NUMLED_C_PORT, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin  = NUMLED_C_IO3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
//	GPIO_Init(NUMLED_C_PORT1, &GPIO_InitStructure);	
	
	GPIO_SetBits(NUMLED_C_PORT, NUMLED_C_IO1 | NUMLED_C_IO2 | NUMLED_C_IO3 | NUMLED_C_IO4);	//����ܲ���
//	GPIO_SetBits(NUMLED_C_PORT1, NUMLED_C_IO3);	//����ܲ���
}
void NUMLED_Dis(unsigned char num,unsigned char value)
{
	if(value==0)NUMLED_0();
	if(value==1)NUMLED_1();
	if(value==2)NUMLED_2();
	if(value==3)NUMLED_3();
	if(value==4)NUMLED_4();
	if(value==5)NUMLED_5();
	if(value==6)NUMLED_6();
	if(value==7)NUMLED_7();
	if(value==8)NUMLED_8();
	if(value==9)NUMLED_9();

	if(value==16)NUMLED_F();
	if(value==17)NUMLED_NULL() ;   //�ر�

	if(num==1)
	{NUMLED1_ON();NUMLED2_OFF();NUMLED3_OFF();NUMLED4_OFF();}
	if(num==2)
	{NUMLED1_OFF();NUMLED2_ON();NUMLED3_OFF();NUMLED4_OFF();}
	if(num==3)
	{NUMLED1_OFF();NUMLED2_OFF();NUMLED3_ON();NUMLED4_OFF();}
	if(num==4)
	{NUMLED1_OFF();NUMLED2_OFF();NUMLED3_OFF();NUMLED4_ON();}

	if(num==0)
	{NUMLED1_OFF();NUMLED2_OFF();NUMLED3_OFF();NUMLED4_OFF();}
	if(num==5)
	{NUMLED1_ON();NUMLED2_ON();NUMLED3_ON();NUMLED4_ON();}
//	if(num==5)
//	{NUMLED1_ON();NUMLED2_ON();NUMLED3_OFF();}
//	if(num==6)
//	{NUMLED1_OFF();NUMLED2_ON();NUMLED3_ON();}

}




