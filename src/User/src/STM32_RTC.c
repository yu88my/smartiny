/***************************************************
**STM32_RTC.c
***************************************************/  
//#include "STM32Lib\\stm32f10x.h"  
#include "STM32_RTC.h"	  
//ǰ���Ƕ�����ʱ��




//static u8 RTC_Blank=0;
/***********************************
**������:RTCInit
**����:RTC����
**ע������:Ҫ�����Ƿ��ǵ�һ�����ã��Ž��������RTC�趨
			�ж��Ƿ��һ�����ã�ֻ��Ҫ�ж�RTC�󱸼Ĵ���1��ֵ�Ƿ�Ϊ����д���0XA5A5��������ǣ���
			RTC�ǵ�һ���ϵ磬��Ҫ��ʼ��RTC������ʵ��ʱ��ת��ΪRTC����ֵ 
			����FALSE�����RTCû�б���ʼ��.
************************************/
bool RTCInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	if (BKP_ReadBackupRegister(BKP_TIME_SET) != 0xA5A5)
	{
		/* Reset Backup Domain */
		BKP_DeInit();		  
		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		//�ȴ��ⲿ������ ��Ҫ�ȴ��Ƚϳ���ʱ��
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		//ʹ���ⲿ����32768
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

		//����RTC
		RCC_RTCCLKCmd(ENABLE);
		//�ȴ�RTC�Ĵ���ͬ��
		RTC_WaitForSynchro();

		RTC_WaitForLastTask();
		//����RTC�����ж�(���������жϺ�����жϿ�����)
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		RTC_WaitForLastTask();
		//32768����Ԥ��Ƶֵ��32767
		RTC_SetPrescaler(32767); /* Ԥ��ƵֵRTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		//RTC_SetPrescaler(32776);	//�����ҪУ׼����,���޸Ĵ˷�Ƶֵ
		RTC_WaitForLastTask();

		//д��RTC�󱸼Ĵ���1 0xa5a5
		BKP_WriteBackupRegister(BKP_TIME_SET, 0xA5A5);  

		//�����־
		RCC_ClearFlag();

		return FALSE;

	}
	//���RTC�Ѿ�����
	else
	{
		//�ȴ�RTC��APBͬ��
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		//���ϵ���������Ҫ��RTC��ֵת��Ϊʱ��
		RTC2Time();
		//ʹ�����ж� 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
		RTC_WaitForLastTask();
		return TRUE;
	}
}



/******************************
**������:GetRTCTime
**����:��ȡʵ��ʱ��
**ע�������ȡֵ���Ž�����ָ���ڣ�ע���ȡǰ��Ҫ�ر�RTC�жϣ�����Ա�����Խ�������
**
*******************************/
static T_STRUCT Real_Time;//ʵʱʱ��ֵ,ֻ����ģ�����,�ⲿ��ȡʱ��һ��ʹ��GetRTCTime����
void GetRTCTime(T_STRUCT* time)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�
	RTC_WaitForLastTask();

	time->year=Real_Time.year;
	time->month=Real_Time.month;
	time->day=Real_Time.day;
	time->hour=Real_Time.hour;
	time->minute=Real_Time.minute;
	time->sec=Real_Time.sec;
	time->date=Real_Time.date;

	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}

/***********************************
**������:GetDate
**����:��������
**ע������:�ײ������㷨,���һ������GPS��ȡ���ں���������.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
} 

/*****************
**������:RTC2Time
**����:��RTC�ڵļ�����ת��Ϊʵ��ʱ��  ����Ϊ  �Ĵ����ڵ�������
**ע������:����RTCֻ�ܴ洢�룬����ÿ����������ʱ�򶼰������մ浽����Ĵ����ڡ�
** ��ÿ��ʱ�ӿ��磬���ȶ�ȡ֮ǰ�������գ������RTC�ڵ�CLK�Ƿ����һ�죬����ڣ�����б�Ҫ������ʹ�Ĵ����ڵ����ڸ��¡�
** ��Щ��Ҫ��RTC�ж�ǰ�������Ȼ��RTC�ж��ڣ���ʱֵ��Զ�������0x0001517f��һ�յ���������
******************/
const u8 Month2Day_Tab[12]={31,28,31,30,31,30,31,31,30,31,30,31} ;
void RTC2Time(void)
{
	u32 count;
	u8 tmp,change=0;

	Real_Time.year=BKP_ReadBackupRegister(BKP_TIME_YEAR);//��ֵ
	Real_Time.month=BKP_ReadBackupRegister(BKP_TIME_MONTH);//��ֵ
	Real_Time.day=BKP_ReadBackupRegister(BKP_TIME_DAY);//��ֵ
	Real_Time.date=BKP_ReadBackupRegister(BKP_TIME_DATE);//����ֵ

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//Ϊ�˱�������������������,�����RTC���ж�������
	count=RTC_GetCounter();
	//�����µ�������
	while (count>=0x0001517f)	 //�ϴιص絽���ο�Խ��һ������
	{
		change=1;
		count-=0x0001517f;
		//�����Լ�
		if ((++Real_Time.date)>=8)
			Real_Time.date=1;

		//�����2�£���������(����Ҫ����2099�Ժ��400��һ����
		if (Real_Time.month==2)
		{
			if (Real_Time.year%4)
				tmp=28;
			else
				tmp=29;
		}
		else
		{
			tmp=Month2Day_Tab[Real_Time.month-1];
		}
		if ((++Real_Time.day)>tmp)
		{
			Real_Time.day=1;

			if ((++Real_Time.month)>12)
			{
				Real_Time.month=1;

				if ((++Real_Time.year)>=100)
				{
					Real_Time.year=0;
				}
			}
		}
	}
	//�����µ�ʱ����
	Real_Time.hour=count/3600;
	Real_Time.minute=(count%3600)/60;
	Real_Time.sec=(count%3600)%60;

	//�����Խ��һ�죬������Ҫ��ؼĴ�����
	if (change)
	{
		RTC_SetCounter(count);
		BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
		BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
		BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
		BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
	}
	//���´�RTC�ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/*********************************
**������:SetRTCTime
**����:����ʱ�䣬���˰�Real_Time��ֵ�ı���,��Ҫ��ʱ����ת��ΪRTC����ֵ,�����մ浽�󱸼Ĵ�����
**ע������:�����ڻ��Զ����������ռ�������,���ҷ��ص�*time��
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�

	RTC_WaitForLastTask();
	//��ʱ��ֵ��Real_Time��
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//��������
	time->date=Real_Time.date=GetDate(time);

	//���µ������մ浽����Ĵ�����

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//�����µ�RTC countֵ
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}
/**************************************************************
** ������:RTCTick
** ����:RTC��������
** ע������:�˺�����RTC���ж��е���,�����ÿ�����ж���ˢ�µ�ǰʱ��
***************************************************************/
void RTCTick(void)
{
	u8 tmp;
	if ((++Real_Time.sec)>59)
	{
		Real_Time.sec=0;
		if ((++Real_Time.minute)>59)
		{
			Real_Time.minute=0;
			if ((++Real_Time.hour)>23)
			{
				Real_Time.hour=0;
				//�����Լ�
				if ((++Real_Time.date)>=8)
					Real_Time.date=1;
				//--�洢�µ�����
				BKP_WriteBackupRegister(BKP_DR5,Real_Time.date);

				//�����2�£���������(����Ҫ����2099�Ժ��400��һ����
				if (Real_Time.month==2)
				{
					if (Real_Time.year%4)
						tmp=28;
					else
						tmp=29;
				}
				else
				{
					tmp=Month2Day_Tab[Real_Time.month-1];
				}
				if ((++Real_Time.day)>tmp)
				{
					Real_Time.day=1;
					if ((++Real_Time.month)>12)
					{
						Real_Time.month=1;

						if ((++Real_Time.year)>99)
						{

							Real_Time.year=0;
						}
						//--�����µ���
						BKP_WriteBackupRegister(BKP_DR2,Real_Time.year);
					}
					//--�����µ���
					BKP_WriteBackupRegister(BKP_DR3,Real_Time.month);
				}
				//--�����µ���
				BKP_WriteBackupRegister(BKP_DR4,Real_Time.day);
			}
		}
	}
}





