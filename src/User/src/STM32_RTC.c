/***************************************************
**STM32_RTC.c
***************************************************/  
//#include "STM32Lib\\stm32f10x.h"  
#include "STM32_RTC.h"	  
//前提是都给以时钟




//static u8 RTC_Blank=0;
/***********************************
**函数名:RTCInit
**功能:RTC设置
**注意事项:要根据是否是第一次设置，才进入下面的RTC设定
			判断是否第一次设置，只需要判断RTC后备寄存器1的值是否为事先写入的0XA5A5，如果不是，则
			RTC是第一次上电，需要初始化RTC，并把实际时间转化为RTC计数值 
			返回FALSE则代表RTC没有被初始化.
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
		//等待外部晶振震荡 需要等待比较长的时间
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		//使用外部晶振32768
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

		//允许RTC
		RCC_RTCCLKCmd(ENABLE);
		//等待RTC寄存器同步
		RTC_WaitForSynchro();

		RTC_WaitForLastTask();
		//允许RTC的秒中断(还有闹钟中断和溢出中断可设置)
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		RTC_WaitForLastTask();
		//32768晶振预分频值是32767
		RTC_SetPrescaler(32767); /* 预分频值RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		//RTC_SetPrescaler(32776);	//如果需要校准晶振,可修改此分频值
		RTC_WaitForLastTask();

		//写入RTC后备寄存器1 0xa5a5
		BKP_WriteBackupRegister(BKP_TIME_SET, 0xA5A5);  

		//清除标志
		RCC_ClearFlag();

		return FALSE;

	}
	//如果RTC已经设置
	else
	{
		//等待RTC与APB同步
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		//刚上电的情况，需要把RTC的值转换为时间
		RTC2Time();
		//使能秒中断 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
		RTC_WaitForLastTask();
		return TRUE;
	}
}



/******************************
**函数名:GetRTCTime
**功能:获取实际时间
**注意事项：获取值将放进参数指针内，注意获取前需要关闭RTC中断，则可以避免秒越界的问题
**
*******************************/
static T_STRUCT Real_Time;//实时时间值,只允许本模块调用,外部获取时间一律使用GetRTCTime函数
void GetRTCTime(T_STRUCT* time)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断
	RTC_WaitForLastTask();

	time->year=Real_Time.year;
	time->month=Real_Time.month;
	time->day=Real_Time.day;
	time->hour=Real_Time.hour;
	time->minute=Real_Time.minute;
	time->sec=Real_Time.sec;
	time->date=Real_Time.date;

	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
}

/***********************************
**函数名:GetDate
**功能:计算星期
**注意事项:白菜星期算法,这个一般用在GPS获取日期后计算出星期.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
} 

/*****************
**函数名:RTC2Time
**功能:把RTC内的计数器转换为实际时间  基数为  寄存器内的年月日
**注意事项:由于RTC只能存储秒，所以每次日跳进的时候都把年月日存到掉电寄存器内。
** 而每次时钟开电，则先读取之前的年月日，并检测RTC内的CLK是否大于一天，如大于，则进行必要的运算使寄存器内的日期更新。
** 这些都要在RTC中断前处理掉，然后RTC中断内，计时值永远不会大于0x0001517f（一日的秒总数）
******************/
const u8 Month2Day_Tab[12]={31,28,31,30,31,30,31,31,30,31,30,31} ;
void RTC2Time(void)
{
	u32 count;
	u8 tmp,change=0;

	Real_Time.year=BKP_ReadBackupRegister(BKP_TIME_YEAR);//年值
	Real_Time.month=BKP_ReadBackupRegister(BKP_TIME_MONTH);//月值
	Real_Time.day=BKP_ReadBackupRegister(BKP_TIME_DAY);//日值
	Real_Time.date=BKP_ReadBackupRegister(BKP_TIME_DATE);//星期值

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//为了避免代码重入引起的问题,这里吧RTC秒中断屏蔽了
	count=RTC_GetCounter();
	//计算新的年月日
	while (count>=0x0001517f)	 //上次关电到本次跨越了一天以上
	{
		change=1;
		count-=0x0001517f;
		//星期自加
		if ((++Real_Time.date)>=8)
			Real_Time.date=1;

		//如果是2月，计算闰年(不需要考虑2099以后的400年一非润）
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
	//计算新的时分秒
	Real_Time.hour=count/3600;
	Real_Time.minute=(count%3600)/60;
	Real_Time.sec=(count%3600)%60;

	//如果跨越了一天，则计算后，要存回寄存器内
	if (change)
	{
		RTC_SetCounter(count);
		BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
		BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
		BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
		BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
	}
	//重新打开RTC中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/*********************************
**函数名:SetRTCTime
**功能:设置时间，除了把Real_Time的值改变外,还要把时分秒转换为RTC计数值,年月日存到后备寄存器上
**注意事项:函数内会自动根据年月日计算星期,并且返回到*time上
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断

	RTC_WaitForLastTask();
	//付时间值到Real_Time上
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//计算星期
	time->date=Real_Time.date=GetDate(time);

	//把新的年月日存到掉电寄存器上

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//计算新的RTC count值
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
}
/**************************************************************
** 函数名:RTCTick
** 功能:RTC的秒跳动
** 注意事项:此函数在RTC秒中断中调用,则可在每个秒中断中刷新当前时间
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
				//星期自加
				if ((++Real_Time.date)>=8)
					Real_Time.date=1;
				//--存储新的星期
				BKP_WriteBackupRegister(BKP_DR5,Real_Time.date);

				//如果是2月，计算闰年(不需要考虑2099以后的400年一非润）
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
						//--储存新的年
						BKP_WriteBackupRegister(BKP_DR2,Real_Time.year);
					}
					//--储存新的月
					BKP_WriteBackupRegister(BKP_DR3,Real_Time.month);
				}
				//--储存新的日
				BKP_WriteBackupRegister(BKP_DR4,Real_Time.day);
			}
		}
	}
}





